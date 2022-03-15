/* 
    *note: DecodeDxt5Part function is referenced from ETCPAK project
*/

#include <cassert>
#include <inttypes.h>
#include <string.h>
#include <stdint.h>

#include "ttc_wrapper.h"

#include "../../Bitmap.hpp"
#include "BlockDataPublic.hpp"

struct Data
{
    const uint32_t* src;
    unsigned int width;
    unsigned int lines;
    unsigned int offset;
};

struct compressWrapper
{
    unsigned int offset;
    unsigned int lines;
    unsigned int m_lines;
    unsigned int m_linesLeft;
    bool done;
    Bitmap* m_current;
    uint32_t* m_block;
    uint32_t* m_data;
    unsigned int m_size_x;
    unsigned int m_size_y;
};

void ttcEncodeDXT5( uint32_t* bitmap, struct TTCFrameProps *info)
{
    bool useHeuristics = true;

    compressWrapper wp;
    
    auto bmp = (Bitmap*) bitmap;
    wp.m_current = new Bitmap(bmp->Size());

    wp.m_current->m_block = bmp->m_block;   
    wp.m_current->m_data = bmp->m_data;
    wp.m_current->m_lines = bmp->m_lines;
    wp.m_current->m_linesLeft = bmp->m_linesLeft;
    wp.m_current->m_size = bmp->m_size;

    BlockData::Type type;
    type = BlockData::Dxt5;

    auto bd = new BlockData( bmp->Size(), false, type );
    bd->ProcessRGBA(wp.m_current->Data(), 
                    wp.m_current->Size().x * wp.m_current->Size().y / 16, 
                    0, wp.m_current->Size().x, useHeuristics 
                    );

    uint32_t *data32 = (uint32_t*) bd->m_data;
    info->f_size.x = wp.m_current->Size().x;
    info->f_size.y = wp.m_current->Size().y;
    info->f_dataOffset = 52 + *(data32+12);
    info->src_buf = (uint64_t*) ( bd->m_data + info->f_dataOffset );
}

void ttcEncodeDXT1( uint32_t* bitmap, struct TTCFrameProps *info)
{
    bool useHeuristics = true;

    compressWrapper wp;
    
    auto bmp = (Bitmap*) bitmap;
    wp.m_current = new Bitmap(bmp->Size());

    wp.m_current->m_block = bmp->m_block;   
    wp.m_current->m_data = bmp->m_data;
    wp.m_current->m_lines = bmp->m_lines;
    wp.m_current->m_linesLeft = bmp->m_linesLeft;
    wp.m_current->m_size = bmp->m_size;

    BlockData::Type type;
    type = BlockData::Dxt1;

    Channels channel;
    channel = Channels::RGB;

    auto bd = new BlockData( bmp->Size(), false, type );
    bd->Process( bmp->Data(), bmp->Size().x * bmp->Size().y / 16, 0, 
                 bmp->Size().x, channel, false, useHeuristics 
                );

    uint32_t *data32 = (uint32_t*) bd->m_data;
    info->f_size.x = wp.m_current->Size().x;
    info->f_size.y = wp.m_current->Size().y;
    info->f_dataOffset = 52 + *(data32+12);
    info->src_buf = (uint64_t*) ( bd->m_data + info->f_dataOffset );
}

void decodeDxt5Part( uint64_t a, uint64_t d, uint32_t* dst, uint32_t w )
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

void decodeDxt1Part( uint64_t d, uint32_t* dst, uint32_t w )
{
    uint8_t* in = (uint8_t*)&d;
    uint16_t c0, c1;
    uint32_t idx;
    memcpy( &c0, in, 2 );
    memcpy( &c1, in+2, 2 );
    memcpy( &idx, in+4, 4 );

    uint8_t r0 = ( ( c0 & 0xF800 ) >> 8 ) | ( ( c0 & 0xF800 ) >> 13 );
    uint8_t g0 = ( ( c0 & 0x07E0 ) >> 3 ) | ( ( c0 & 0x07E0 ) >> 9 );
    uint8_t b0 = ( ( c0 & 0x001F ) << 3 ) | ( ( c0 & 0x001F ) >> 2 );

    uint8_t r1 = ( ( c1 & 0xF800 ) >> 8 ) | ( ( c1 & 0xF800 ) >> 13 );
    uint8_t g1 = ( ( c1 & 0x07E0 ) >> 3 ) | ( ( c1 & 0x07E0 ) >> 9 );
    uint8_t b1 = ( ( c1 & 0x001F ) << 3 ) | ( ( c1 & 0x001F ) >> 2 );

    uint32_t dict[4];

    dict[0] = 0xFF000000 | ( b0 << 16 ) | ( g0 << 8 ) | r0;
    dict[1] = 0xFF000000 | ( b1 << 16 ) | ( g1 << 8 ) | r1;

    uint32_t r, g, b;
    if( c0 > c1 )
    {
        r = (2*r0+r1)/3;
        g = (2*g0+g1)/3;
        b = (2*b0+b1)/3;
        dict[2] = 0xFF000000 | ( b << 16 ) | ( g << 8 ) | r;
        r = (2*r1+r0)/3;
        g = (2*g1+g0)/3;
        b = (2*b1+b0)/3;
        dict[3] = 0xFF000000 | ( b << 16 ) | ( g << 8 ) | r;
    }
    else
    {
        r = (int(r0)+r1)/2;
        g = (int(g0)+g1)/2;
        b = (int(b0)+b1)/2;
        dict[2] = 0xFF000000 | ( b << 16 ) | ( g << 8 ) | r;
        dict[3] = 0xFF000000;
    }

    memcpy( dst+0, dict + (idx & 0x3), 4 );
    idx >>= 2;
    memcpy( dst+1, dict + (idx & 0x3), 4 );
    idx >>= 2;
    memcpy( dst+2, dict + (idx & 0x3), 4 );
    idx >>= 2;
    memcpy( dst+3, dict + (idx & 0x3), 4 );
    idx >>= 2;
    dst += w;

    memcpy( dst+0, dict + (idx & 0x3), 4 );
    idx >>= 2;
    memcpy( dst+1, dict + (idx & 0x3), 4 );
    idx >>= 2;
    memcpy( dst+2, dict + (idx & 0x3), 4 );
    idx >>= 2;
    memcpy( dst+3, dict + (idx & 0x3), 4 );
    idx >>= 2;
    dst += w;

    memcpy( dst+0, dict + (idx & 0x3), 4 );
    idx >>= 2;
    memcpy( dst+1, dict + (idx & 0x3), 4 );
    idx >>= 2;
    memcpy( dst+2, dict + (idx & 0x3), 4 );
    idx >>= 2;
    memcpy( dst+3, dict + (idx & 0x3), 4 );
    idx >>= 2;
    dst += w;

    memcpy( dst+0, dict + (idx & 0x3), 4 );
    idx >>= 2;
    memcpy( dst+1, dict + (idx & 0x3), 4 );
    idx >>= 2;
    memcpy( dst+2, dict + (idx & 0x3), 4 );
    idx >>= 2;
    memcpy( dst+3, dict + (idx & 0x3), 4 );
}

void ttcDecodeDXT5(struct TTCFrameProps *m)
{
    m->dst_buf = new uint32_t [m->f_size.x*m->f_size.y];
    uint32_t* dst = m->dst_buf;
    for( int y=0; y<m->f_size.y/4; y++ )
    {
        for( int x=0; x<m->f_size.x/4; x++ )
        {
            uint64_t a = *m->src_buf++;
            uint64_t d = *m->src_buf++;
            decodeDxt5Part( a, d, dst, m->f_size.x );
            dst += 4;
        }
        dst += m->f_size.x*3;
    }
}

void ttcDecodeDXT1(struct TTCFrameProps *m)
{
    m->dst_buf = new uint32_t [m->f_size.x*m->f_size.y];
    uint32_t* dst = m->dst_buf;
    for( int y=0; y<m->f_size.y/4; y++ )
    {
        for( int x=0; x<m->f_size.x/4; x++ )
        {
            uint64_t d = *m->src_buf++;
            decodeDxt1Part( d, dst, m->f_size.x );
            dst += 4;
        }
        dst += m->f_size.x*3;
    }
}
