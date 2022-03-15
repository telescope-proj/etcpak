#include <assert.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <png.h>

#include "include/ttc_wrapper.h"
#include "../mmap.hpp"

void ttcReadImg(char* input, struct TTCFrameProps *m)
{
    FILE* f_file = fopen( input, "rb" );
    assert( f_file );
    fseek( f_file, 0, SEEK_END );
    size_t f_maplen = ftell( f_file );
    fseek( f_file, 0, SEEK_SET );
    void *ptr=(int *)0;
    m->f_data = (uint8_t*)mmap( ptr, f_maplen, PROT_READ, 
                                MAP_SHARED, fileno( f_file ), 0);
    uint32_t *data32 = (uint32_t*) m->f_data;
    m->f_size.y = *(data32+6);
    m->f_size.x = *(data32+7);
    m->f_dataOffset = 52 + *(data32+12);
    m->src_buf = (uint64_t*)( m->f_data + m->f_dataOffset );
    fclose(f_file);
}

void ttcRenderImg(char* output, int32_t x, int32_t y, uint32_t * dst_buf)
{
    FILE* f = fopen( output, "wb" );
    assert( f );
    png_structp png_ptr = png_create_write_struct(  PNG_LIBPNG_VER_STRING, 
                                                    NULL, NULL, NULL );
    png_infop info_ptr = png_create_info_struct( png_ptr );
    setjmp( png_jmpbuf( png_ptr ) );
    png_init_io( png_ptr, f );
    png_set_IHDR(   png_ptr, info_ptr, x, y, 8, PNG_COLOR_TYPE_RGB_ALPHA, 
                    PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, 
                    PNG_FILTER_TYPE_BASE );
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

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr,"main: no argument specified\n");
        fprintf(stderr,"main: [dxt1] [path to input] [path to save png]\n");
        fprintf(stderr,"main: [dxt5] [path to input] [path to save png]\n");
        exit(1);
    }
    float sdt, edt, ddt1, ddt2; //* decode timing var
    float set, eet, det1, det2; //* encode timing var
    if (!strcmp(argv[1], "dxt5"))
    {
        if (argc < 4) {
            fprintf(stderr,"main: invalid number of arguments\n");
            exit(1);
        }
        struct TTCFrameProps * m1 = calloc(1, sizeof(*m1));

        uint32_t *img_ptr = readPng(argv[2]);

        set = (float)clock()/CLOCKS_PER_SEC;
        ttcEncodeDXT5(img_ptr, m1);
        eet = (float)clock()/CLOCKS_PER_SEC;
        det1 = eet - set;
        printf("DXT5 Encoding Time : %f\n", det1);
        //* ttcReadImg(argv[2], &m1);

        sdt = (float)clock()/CLOCKS_PER_SEC;
        ttcDecodeDXT5(m1);  
        edt = (float)clock()/CLOCKS_PER_SEC;
        ddt1 = edt - sdt;

        printf("DXT5 Decoding Time : %f\n", ddt1);
        ttcRenderImg(argv[3], m1->f_size.x, m1->f_size.y, m1->dst_buf);
    }
    else if (!strcmp(argv[1], "dxt1"))
    {
        if (argc < 4) {
            fprintf(stderr,"main: invalid number of arguments\n");
            exit(1);
        }
        struct TTCFrameProps * m2 = calloc(1, sizeof(*m2));

        uint32_t *img_ptr = readPng(argv[2]);
        
        set = (float)clock()/CLOCKS_PER_SEC;
        ttcEncodeDXT5(img_ptr, m2);
        eet = (float)clock()/CLOCKS_PER_SEC;
        det1 = eet - set;
        printf("DXT1 Encoding Time : %f\n", det1);
        //* ttcReadImg(argv[2], m2);

        sdt = (float)clock()/CLOCKS_PER_SEC;
        ttcDecodeDXT1(m2);
        edt = (float)clock()/CLOCKS_PER_SEC;
        ddt2 = edt - sdt;

        printf("DXT1 Decoding Time : %f\n", ddt2);
        ttcRenderImg(argv[3], m2->f_size.x, m2->f_size.y, m2->dst_buf);
    }
    else{printf("invalid arguments");}
    return 0;
}