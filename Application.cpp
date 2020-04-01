#include <future>
#include <stdio.h>
#include <limits>
#include <math.h>
#include <memory>
#include <string.h>

#include "getopt/getopt.h"

#include "Bitmap.hpp"
#include "BlockData.hpp"
#include "DataProvider.hpp"
#include "Debug.hpp"
#include "Dither.hpp"
#include "Error.hpp"
#include "System.hpp"
#include "TaskDispatch.hpp"
#include "Timing.hpp"

struct DebugCallback_t : public DebugLog::Callback
{
    void OnDebugMessage( const char* msg ) override
    {
        fprintf( stderr, "%s\n", msg );
    }
} DebugCallback;

void Usage()
{
    fprintf( stderr, "Usage: etcpak [options] input.png {output.pvr}\n" );
    fprintf( stderr, "  Options:\n" );
    fprintf( stderr, "  -v          view mode (loads pvr/ktx file, decodes it and saves to png)\n" );
    fprintf( stderr, "  -a          disable alpha channel processing\n" );
    fprintf( stderr, "  -s          display image quality measurements\n" );
    fprintf( stderr, "  -b          benchmark mode\n" );
    fprintf( stderr, "  -m          generate mipmaps\n" );
    fprintf( stderr, "  -d          enable dithering\n" );
    fprintf( stderr, "  --debug     dissect ETC texture\n" );
    fprintf( stderr, "  --etc2      enable ETC2 mode\n" );
    fprintf( stderr, "  --rgba      enable ETC2 RGBA mode\n\n" );
    fprintf( stderr, "Output file name may be unneeded for some modes.\n" );
}

int main( int argc, char** argv )
{
    DebugLog::AddCallback( &DebugCallback );

    bool viewMode = false;
    bool alpha = true;
    bool stats = false;
    bool benchmark = false;
    bool mipmap = false;
    bool dither = false;
    bool debug = false;
    bool etc2 = false;
    bool rgba = false;
    unsigned int cpus = System::CPUCores();

    if( argc < 3 )
    {
        Usage();
        return 1;
    }

    enum Options
    {
        OptDebug,
        OptEtc2,
        OptRgba
    };

    struct option longopts[] = {
        { "debug", no_argument, nullptr, OptDebug },
        { "etc2", no_argument, nullptr, OptEtc2 },
        { "rgba", no_argument, nullptr, OptRgba },
        {}
    };

    int c;
    while( ( c = getopt_long( argc, argv, "vo:asbmd", longopts, nullptr ) ) != -1 )
    {
        switch( c )
        {
        case 'v':
            viewMode = true;
            break;
        case 'a':
            alpha = false;
            break;
        case 's':
            stats = true;
            break;
        case 'b':
            benchmark = true;
            break;
        case 'm':
            mipmap = true;
            break;
        case 'd':
            dither = true;
            break;
        case OptDebug:
            debug = true;
            break;
        case OptEtc2:
            etc2 = true;
            break;
        case OptRgba:
            rgba = true;
            etc2 = true;
            break;
        default:
            break;
        }
    }

    const char* input = nullptr;
    const char* output = nullptr;
    if( benchmark || viewMode || debug )
    {
        if( argc - optind < 1 )
        {
            Usage();
            return 1;
        }

        input = argv[optind];
    }
    else
    {
        if( argc - optind < 2 )
        {
            Usage();
            return 1;
        }

        input = argv[optind];
        output = argv[optind+1];
    }

    if( dither )
    {
        InitDither();
    }

    if( benchmark )
    {
        auto start = GetTime();
        auto bmp = std::make_shared<Bitmap>( input, std::numeric_limits<unsigned int>::max() );
        auto data = bmp->Data();
        auto end = GetTime();
        printf( "Image load time: %0.3f ms\n", ( end - start ) / 1000.f );

        constexpr int NumTasks = 9;
        uint64_t timeData[NumTasks];
        for( int i=0; i<NumTasks; i++ )
        {
            const auto localStart = GetTime();
            const BlockData::Type type = rgba ? BlockData::Etc2_RGBA : ( etc2 ? BlockData::Etc2_RGB : BlockData::Etc1 );
            auto bd = std::make_shared<BlockData>( bmp->Size(), false, type );
            if( rgba )
            {
                bd->ProcessRGBA( bmp->Data(), bmp->Size().x * bmp->Size().y / 16, 0, bmp->Size().x, dither );
            }
            else
            {
                bd->Process( bmp->Data(), bmp->Size().x * bmp->Size().y / 16, 0, bmp->Size().x, Channels::RGB, dither );
            }
            const auto localEnd = GetTime();
            timeData[i] = localEnd - localStart;
        }
        std::sort( timeData, timeData+NumTasks );
        const auto median = timeData[NumTasks/2] / 1000.f;
        printf( "Median compression time for %i runs: %0.3f ms (%0.3f Mpx/s)\n", NumTasks, median, bmp->Size().x * bmp->Size().y / ( median * 1000 ) );
    }
    else if( viewMode )
    {
        auto bd = std::make_shared<BlockData>( input );
        auto out = bd->Decode();
        out->Write( output );
    }
    else if( debug )
    {
        auto bd = std::make_shared<BlockData>( input );
        bd->Dissect();
    }
    else
    {
        DataProvider dp( input, mipmap );
        auto num = dp.NumberOfParts();

        BlockData::Type type;
        if( etc2 )
        {
            if( rgba && dp.Alpha() )
            {
                type = BlockData::Etc2_RGBA;
            }
            else
            {
                type = BlockData::Etc2_RGB;
            }
        }
        else
        {
            type = BlockData::Etc1;
        }

        TaskDispatch taskDispatch( cpus );

        auto bd = std::make_shared<BlockData>( output, dp.Size(), mipmap, type );
        for( int i=0; i<num; i++ )
        {
            auto part = dp.NextPart();

            if( type == BlockData::Etc2_RGBA )
            {
                TaskDispatch::Queue( [part, i, &bd, &dither]()
                {
                    bd->ProcessRGBA( part.src, part.width / 4 * part.lines, part.offset, part.width, dither );
                } );
            }
            else
            {
                TaskDispatch::Queue( [part, i, &bd, &dither]()
                {
                    bd->Process( part.src, part.width / 4 * part.lines, part.offset, part.width, Channels::RGB, dither );
                } );
            }
        }

        TaskDispatch::Sync();

        if( stats )
        {
            auto out = bd->Decode();
            float mse = CalcMSE3( dp.ImageData(), *out );
            printf( "RGB data\n" );
            printf( "  RMSE: %f\n", sqrt( mse ) );
            printf( "  PSNR: %f\n", 20 * log10( 255 ) - 10 * log10( mse ) );
        }

        bd.reset();
    }

    return 0;
}
