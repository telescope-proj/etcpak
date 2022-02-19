/* 
note: DecodeDxt5Part function is referenced from ETCPAC project
*/

#include <cassert>
#include <string.h>
#include <stdint.h>
#include <png.h>

#include "../mmap.hpp"
#include "wrapper.h"

void DecodeDxt5Part( uint64_t a, uint64_t d, uint32_t* dst, uint32_t w )
{
    uint8_t* ain = (uint8_t*)&a;
    uint8_t a0, a1;
    uint64_t aidx = 0;
    memcpy( &a0, ain, 1 );
    memcpy( &a1, ain+1, 1 );
    memcpy( &aidx, ain+2, 6 );
    uint8_t* in = (uint8_t*)&d;
    uint16_t c0, c1;
    uint32_t idx;
    memcpy( &c0, in, 2 );
    memcpy( &c1, in+2, 2 );
    memcpy( &idx, in+4, 4 );

    uint32_t adict[8];
    adict[0] = a0 << 24;
    adict[1] = a1 << 24;
    if( a0 > a1 )
    {
        adict[2] = ( (6*a0+1*a1)/7 ) << 24;
        adict[3] = ( (5*a0+2*a1)/7 ) << 24;
        adict[4] = ( (4*a0+3*a1)/7 ) << 24;
        adict[5] = ( (3*a0+4*a1)/7 ) << 24;
        adict[6] = ( (2*a0+5*a1)/7 ) << 24;
        adict[7] = ( (1*a0+6*a1)/7 ) << 24;
    }
    else
    {
        adict[2] = ( (4*a0+1*a1)/5 ) << 24;
        adict[3] = ( (3*a0+2*a1)/5 ) << 24;
        adict[4] = ( (2*a0+3*a1)/5 ) << 24;
        adict[5] = ( (1*a0+4*a1)/5 ) << 24;
        adict[6] = 0;
        adict[7] = 0xFF000000;
    }

    uint8_t r0 = ( ( c0 & 0xF800 ) >> 8 ) | ( ( c0 & 0xF800 ) >> 13 );
    uint8_t g0 = ( ( c0 & 0x07E0 ) >> 3 ) | ( ( c0 & 0x07E0 ) >> 9 );
    uint8_t b0 = ( ( c0 & 0x001F ) << 3 ) | ( ( c0 & 0x001F ) >> 2 );

    uint8_t r1 = ( ( c1 & 0xF800 ) >> 8 ) | ( ( c1 & 0xF800 ) >> 13 );
    uint8_t g1 = ( ( c1 & 0x07E0 ) >> 3 ) | ( ( c1 & 0x07E0 ) >> 9 );
    uint8_t b1 = ( ( c1 & 0x001F ) << 3 ) | ( ( c1 & 0x001F ) >> 2 );

    uint32_t dict[4];

    dict[0] = ( b0 << 16 ) | ( g0 << 8 ) | r0;
    dict[1] = ( b1 << 16 ) | ( g1 << 8 ) | r1;

    uint32_t r, g, b;
    if( c0 > c1 )
    {
        r = (2*r0+r1)/3;
        g = (2*g0+g1)/3;
        b = (2*b0+b1)/3;
        dict[2] = ( b << 16 ) | ( g << 8 ) | r;
        r = (2*r1+r0)/3;
        g = (2*g1+g0)/3;
        b = (2*b1+b0)/3;
        dict[3] = ( b << 16 ) | ( g << 8 ) | r;
    }
    else
    {
        r = (int(r0)+r1)/2;
        g = (int(g0)+g1)/2;
        b = (int(b0)+b1)/2;
        dict[2] = ( b << 16 ) | ( g << 8 ) | r;
        dict[3] = 0;
    }
    dst[0] = dict[idx & 0x3] | adict[aidx & 0x7];
    idx >>= 2;
    aidx >>= 3;
    dst[1] = dict[idx & 0x3] | adict[aidx & 0x7];
    idx >>= 2;
    aidx >>= 3;
    dst[2] = dict[idx & 0x3] | adict[aidx & 0x7];
    idx >>= 2;
    aidx >>= 3;
    dst[3] = dict[idx & 0x3] | adict[aidx & 0x7];
    idx >>= 2;
    aidx >>= 3;
    dst += w;
    dst[0] = dict[idx & 0x3] | adict[aidx & 0x7];
    idx >>= 2;
    aidx >>= 3;
    dst[1] = dict[idx & 0x3] | adict[aidx & 0x7];
    idx >>= 2;
    aidx >>= 3;
    dst[2] = dict[idx & 0x3] | adict[aidx & 0x7];
    idx >>= 2;
    aidx >>= 3;
    dst[3] = dict[idx & 0x3] | adict[aidx & 0x7];
    idx >>= 2;
    aidx >>= 3;
    dst += w;

    dst[0] = dict[idx & 0x3] | adict[aidx & 0x7];
    idx >>= 2;
    aidx >>= 3;
    dst[1] = dict[idx & 0x3] | adict[aidx & 0x7];
    idx >>= 2;
    aidx >>= 3;
    dst[2] = dict[idx & 0x3] | adict[aidx & 0x7];
    idx >>= 2;
    aidx >>= 3;
    dst[3] = dict[idx & 0x3] | adict[aidx & 0x7];
    idx >>= 2;
    aidx >>= 3;
    dst += w;

    dst[0] = dict[idx & 0x3] | adict[aidx & 0x7];
    idx >>= 2;
    aidx >>= 3;
    dst[1] = dict[idx & 0x3] | adict[aidx & 0x7];
    idx >>= 2;
    aidx >>= 3;
    dst[2] = dict[idx & 0x3] | adict[aidx & 0x7];
    idx >>= 2;
    aidx >>= 3;
    dst[3] = dict[idx & 0x3] | adict[aidx & 0x7];
}

void ReadImg(char* input, struct mstruct *m){
    m->m_file = fopen( input, "rb" );
    assert( m->m_file );
    fseek( m->m_file, 0, SEEK_END );
    m->m_maplen = ftell( m->m_file );
    fseek( m->m_file, 0, SEEK_SET );
    m->m_data = (uint8_t*)mmap( nullptr, m->m_maplen, PROT_READ, MAP_SHARED, fileno( m->m_file ), 0 );
    uint32_t *data32 = (uint32_t*) m->m_data;
    m->m_size.y = *(data32+6);
    m->m_size.x = *(data32+7);
    m->m_dataOffset = 52 + *(data32+12);
    m->src_buf = (uint64_t*)( m->m_data + m->m_dataOffset );
    fclose(m->m_file);
}

void DecodeDXT5(struct mstruct *m){
    m->dst_buf = new uint32_t [m->m_size.x*m->m_size.y];
    uint32_t* dst = m->dst_buf;
    for( int y=0; y<m->m_size.y/4; y++ )
    {
        for( int x=0; x<m->m_size.x/4; x++ )
        {
            uint64_t a = *m->src_buf++;
            uint64_t d = *m->src_buf++;
            DecodeDxt5Part( a, d, dst, m->m_size.x );
            dst += 4;
        }
        dst += m->m_size.x*3;
    }
}

void RenderImg(char* output, int32_t x, int32_t y, uint32_t * dst_buf){
    FILE* f = fopen( output, "wb" );
    assert( f );
    png_structp png_ptr = png_create_write_struct( PNG_LIBPNG_VER_STRING, NULL, NULL, NULL );
    png_infop info_ptr = png_create_info_struct( png_ptr );
    setjmp( png_jmpbuf( png_ptr ) );
    png_init_io( png_ptr, f );
    png_set_IHDR( png_ptr, info_ptr, x, y, 8, PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE );
    png_write_info( png_ptr, info_ptr );
    uint32_t* ptr = dst_buf;
    for( int i=0; i<y; i++ )
    {
        png_write_rows( png_ptr, (png_bytepp)(&ptr), 1 );
        ptr += x;
    }
    png_write_end( png_ptr, info_ptr );
    png_destroy_write_struct( &png_ptr, &info_ptr );
    fclose( f );
}

// void WrapCompressDxt5(  char* input, char* output )
// {       
//     bool mipmap = false;
//     bool dither = false;
//     bool rgba = false;
//     bool dxtc = true;
//     bool linearize = true;
//     bool useHeuristics = true;
//     const char* alpha = nullptr;
//     unsigned int cpus = System::CPUCores();

//     DataProvider dp( input, mipmap, !dxtc, linearize );
//     auto num = dp.NumberOfParts();
//     printf("num/dp.numberofparts = %d\n\n", num);

//     BlockData::Type type;
//     type = BlockData::Dxt5; // *if alpha == true then use Dxt5

//     TaskDispatch taskDispatch( cpus );

//     auto bd = std::make_shared<BlockData>( output, dp.Size(), mipmap, type );
//     BlockDataPtr bda;
//     if( alpha && dp.Alpha() && !rgba )
//     {
//         bda = std::make_shared<BlockData>( alpha, dp.Size(), mipmap, type );
//     }
//     for( int i=0; i<num; i++ )
//     {
//         auto part = dp.NextPart();
//         TaskDispatch::Queue( [part, i, &bd, &dither, useHeuristics]()
//         {
//             bd->ProcessRGBA( part.src, part.width / 4 * part.lines, part.offset, part.width, useHeuristics );
//         } );
//     }
//     TaskDispatch::Sync();
//     //CompressDxt5(src, dst, blocks, width );
// }