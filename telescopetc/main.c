#include <assert.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "wrapper.h"

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr,"main: no argument specified\n");
        fprintf(stderr,"main: [dxt1] [path to pvr] [path to save png]\n");
        fprintf(stderr,"main: [dxt5] [path to pvr] [path to save png]\n");
        exit(1);
    }
    float st, et, dt1, dt2;
    if (!strcmp(argv[1], "dxt5"))
    {
        if (argc < 4) {
            fprintf(stderr,"main: invalid number of arguments\n");
            exit(1);
        }
        struct mstruct m1;
        ReadImg(argv[2], &m1);
        st = (float)clock()/CLOCKS_PER_SEC;
        DecodeDXT5(&m1);  
        et = (float)clock()/CLOCKS_PER_SEC;
        dt1 = et - st;
        printf("DXT5 Decoding Time : %f\n", dt1);
        RenderImg(argv[3], m1.m_size.x, m1.m_size.y, m1.dst_buf);
    }
    else if (!strcmp(argv[1], "dxt1"))
    {
        if (argc < 4) {
            fprintf(stderr,"main: invalid number of arguments\n");
            exit(1);
        }
        struct mstruct m2;
        ReadImg(argv[2], &m2);
        st = (float)clock()/CLOCKS_PER_SEC;
        DecodeDXT1(&m2); 
        et = (float)clock()/CLOCKS_PER_SEC;
        dt2 = et - st;
        printf("DXT1 Decoding Time : %f\n", dt2);
        RenderImg(argv[3], m2.m_size.x, m2.m_size.y, m2.dst_buf);
    }
    else{printf("invalid arguments");}

    return 0;
}
