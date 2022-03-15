#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "lz4/lz4.h"

#include "Bitmap.hpp"
#include "Debug.hpp"

Bitmap::Bitmap( const v2i& size )
    : m_data( new uint32_t[size.x*size.y] )
    , m_block( nullptr )
    , m_lines( 1 )
    , m_linesLeft( size.y / 4 )
    , m_size( size )
    , m_sema( 0 )
{
}

Bitmap::Bitmap( const Bitmap& src, unsigned int lines )
    : m_lines( lines )
    , m_alpha( src.Alpha() )
    , m_sema( 0 )
{
}

Bitmap::~Bitmap()
{
    delete[] m_data;
}

const uint32_t* Bitmap::NextBlock( unsigned int& lines, bool& done )
{
    lines = std::min( m_lines, m_linesLeft );
    auto ret = m_block;
    m_block += m_size.x * 4 * lines;
    m_linesLeft -= lines;
    done = m_linesLeft == 0;
    return ret;
}
