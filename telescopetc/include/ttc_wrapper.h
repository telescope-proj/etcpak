#ifdef __cplusplus
  #define EXPORT_C extern "C"
#else
  #define EXPORT_C
#endif

struct TTCFrameProps {
    void* buffer;
    uint32_t width;
    uint32_t height;
    uint32_t format;
};

// ************************ Encode Functions *********************************

EXPORT_C void ttcDecodeDXT5( void* src, void* dst, uint32_t width,
                              uint32_t height, uint32_t format);

EXPORT_C void ttcDecodeDXT1( void* src, void* dst, uint32_t width,
                              uint32_t height, uint32_t format);

// ************************ Encode Functions *********************************

EXPORT_C void ttcEncodeDXT5( void* src, void* dst, uint32_t width,
                              uint32_t height, uint32_t format);

EXPORT_C void ttcEncodeDXT1( void* src, void* dst, uint32_t width,
                              uint32_t height, uint32_t format);