#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include "wrapper.h"

int main()
{
    struct mstruct * m = calloc(1, sizeof(*m));

    uint32_t *img_ptr = readPng("assets/bright.png");

    float start_compress_time = (float)clock()/CLOCKS_PER_SEC;
    WrapCompressDxt5(img_ptr, m);

    float end_compress_time = (float)clock()/CLOCKS_PER_SEC;
    float diff_time_compress = end_compress_time -  start_compress_time;
    printf("Encoding Time : %f\n", diff_time_compress);

    float start_decompress_time = (float)clock()/CLOCKS_PER_SEC;
    
    DecodeDXT5(m); 

    float end_decompress_time = (float)clock()/CLOCKS_PER_SEC;
    float diff_time_decompress = end_decompress_time -  start_decompress_time;
    printf("Decoding Time : %f\n", diff_time_decompress);

    RenderImg("doggo_out.png", m->m_size.x, m->m_size.y, m->dst_buf);

    return 0;
}
