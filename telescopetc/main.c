#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

#include "wrapper.h"

int main()
{
    /** Code here **/
    struct mstruct * m = calloc(1, sizeof(*m));

    
    // ReadImg("test_compressed.pvr", &m);
    float start_compress_time = (float)clock()/CLOCKS_PER_SEC;
    
    WrapCompressDxt5("screenshot1.png", m);

    //OldWrapCompressDxt5("screenshot1.png","xxx.pvr");
    // *src buffer: m.src_buf | dst buffer: m.dst_buf
    //function that output m struct
    // printf("info->src buf 2 %p\n", m->src_buf);
    // printf("%" PRIu64 "\n", *(m->src_buf) );
    // printf("size x %d  %p\n",m->m_size.x,&m->m_size.x);
    // printf("size y %d  %p\n",m->m_size.y,&m->m_size.y);

    float end_compress_time = (float)clock()/CLOCKS_PER_SEC;
    float diff_time_compress = end_compress_time -  start_compress_time;
    printf("Encoding Time : %f\n", diff_time_compress);

    float start_decompress_time = (float)clock()/CLOCKS_PER_SEC;

    
    
    DecodeDXT5(m); 

    float end_decompress_time = (float)clock()/CLOCKS_PER_SEC;
    float diff_time_decompress = end_decompress_time -  start_decompress_time;
    printf("Decoding Time : %f\n", diff_time_decompress);



    //RenderImg("ourDecode8.png", m->m_size.x, m->m_size.y, m->dst_buf);

    return 0;
}
