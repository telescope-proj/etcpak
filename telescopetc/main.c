#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include "wrapper.h"

int main()
{
    /** Code here **/
    struct mstruct m;

    ReadImg("testdxtc.pvr", &m);
    float st = (float)clock()/CLOCKS_PER_SEC;
    // *src buffer: m.src_buf | dst buffer: m.dst_buf
    DecodeDXT5(&m); 
    // DecodeDXT1(&m); 
    float et = (float)clock()/CLOCKS_PER_SEC;
    float dt = et - st;
    printf("Decoding Time : %f\n", dt);
    RenderImg("ourDecode.png", m.m_size.x, m.m_size.y, m.dst_buf);

    return 0;
}
