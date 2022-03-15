#ifdef __cplusplus
  #define EXPORT_C extern "C"
#else
  #define EXPORT_C
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
  uint32_t*       dst_buf;
  uint64_t*       src_buf;
};

EXPORT_C void ttcDecodeDXT5(struct TTCFrameProps *m);

EXPORT_C void ttcDecodeDXT1(struct TTCFrameProps *m);

EXPORT_C void ttcEncodeDXT5( uint32_t* bitmap, struct TTCFrameProps *info);