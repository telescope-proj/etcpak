#ifdef __cplusplus
  #define EXPORT_C extern "C"
#else
  #define EXPORT_C
#endif

#ifdef __cplusplus
#include "mmap.hpp"
#endif

struct TTCDim {
    int32_t x;
    int32_t y;
};

struct TTCFrameProps
{
  uint8_t*        f_data;
  struct  TTCDim  f_size;
  size_t          f_dataOffset;
  FILE*           f_file;
  size_t          f_maplen;
  uint32_t*       dst_buf;
  uint64_t*       src_buf;
};

EXPORT_C void ttcRenderImg( char* output, int32_t x, int32_t y,
                            uint32_t * data_buf);

EXPORT_C void ttcReadImg(char* input, struct TTCFrameProps *m);

EXPORT_C void ttcDecodeDXT5(struct TTCFrameProps *m);

EXPORT_C void ttcDecodeDXT1(struct TTCFrameProps *m);