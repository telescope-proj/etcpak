#include <assert.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
//#include "wrapper.h"

#include "include/ttc_wrapper.h"

int main(int argc, char *argv[])
{
    // struct TTCFrameProps * m = calloc(1, sizeof(*m));

    // uint32_t *img_ptr = readPng("assets/bright.png");

    // float start_compress_time = (float)clock()/CLOCKS_PER_SEC;
    // WrapCompressDxt5(img_ptr, m);

    // float end_compress_time = (float)clock()/CLOCKS_PER_SEC;
    // float diff_time_compress = end_compress_time -  start_compress_time;
    // printf("Encoding Time : %f\n", diff_time_compress);

    // float start_decompress_time = (float)clock()/CLOCKS_PER_SEC;
    
    // ttcDecodeDXT5(m); 

    // float end_decompress_time = (float)clock()/CLOCKS_PER_SEC;
    // float diff_time_decompress = end_decompress_time -  start_decompress_time;
    // printf("Decoding Time : %f\n", diff_time_decompress);

    // ttcRenderImg("doggo_out3.png", m->f_size.x, m->f_size.y, m->dst_buf);
/////////////////////////////////////////////////////////////////////////////////////////
    if (argc < 2) {
        fprintf(stderr,"main: no argument specified\n");
        fprintf(stderr,"main: [dxt1] [path to input] [path to save png]\n");
        fprintf(stderr,"main: [dxt5] [path to input] [path to save png]\n");
        exit(1);
    }
    float st, et, dt1, dt2;
    if (!strcmp(argv[1], "dxt5"))
    {
        if (argc < 4) {
            fprintf(stderr,"main: invalid number of arguments\n");
            exit(1);
        }
        struct TTCFrameProps * m1 = calloc(1, sizeof(*m1));
        uint32_t *img_ptr = readPng(argv[2]);
        float start_compress_time = (float)clock()/CLOCKS_PER_SEC;
        WrapCompressDxt5(img_ptr, m1);
        float end_compress_time = (float)clock()/CLOCKS_PER_SEC;
        float diff_time_compress = end_compress_time -  start_compress_time;
        printf("Encoding Time : %f\n", diff_time_compress);
        // ttcReadImg(argv[2], &m1);
        st = (float)clock()/CLOCKS_PER_SEC;
        ttcDecodeDXT5(m1);  
        et = (float)clock()/CLOCKS_PER_SEC;
        dt1 = et - st;
        printf("DXT5 Decoding Time : %f\n", dt1);
        ttcRenderImg(argv[3], m1->f_size.x, m1->f_size.y, m1->dst_buf);
    }
    // else if (!strcmp(argv[1], "dxt1"))
    // {
    //     if (argc < 4) {
    //         fprintf(stderr,"main: invalid number of arguments\n");
    //         exit(1);
    //     }
    //     struct TTCFrameProps * m2 = calloc(1, sizeof(*m));
    //     // ttcReadImg(argv[2], &m2);
    //     st = (float)clock()/CLOCKS_PER_SEC;
    //     ttcDecodeDXT1(&m2); 
    //     et = (float)clock()/CLOCKS_PER_SEC;
    //     dt2 = et - st;
    //     printf("DXT1 Decoding Time : %f\n", dt2);
    //     ttcRenderImg(argv[3], m2->f_size.x, m2->f_size.y, m2->dst_buf);
    // }
    else{printf("invalid arguments");}
    return 0;
}
