#ifdef __cplusplus
  #define EXPORT_C extern "C"
#else
  #define EXPORT_C
#endif

#ifdef __cplusplus
// #include "../mmap.hpp"

#endif
struct Vector {
    int32_t x;
    int32_t y;
};

struct mstruct
{
  uint8_t* m_data;
  struct Vector m_size;
  size_t m_dataOffset;
  FILE* m_file;
  size_t m_maplen;
  uint32_t* dst_buf;
  uint64_t* src_buf;
};

EXPORT_C  void  ReadImg(char* input, struct mstruct *m);
EXPORT_C  void  RenderImg(char* output, int32_t x, int32_t y, uint32_t * data_buf);
EXPORT_C  void  DecodeDXT5(struct mstruct *m);
EXPORT_C  void  DecodeDXT1(struct mstruct *m);