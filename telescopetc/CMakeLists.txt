include_directories(include)
set(SOURCES
    src/ttc_wrapper.cpp
    src/mmap.cpp
    src/BlockDataPublic.cpp
    ../Timing.cpp
    ../TaskDispatch.cpp
    ../System.cpp
    ../ProcessDxtc.cpp
    ../Dither.cpp
    ../BlockData.cpp
    ../ProcessRGB.cpp
    ../Tables.cpp
    ../Bitmap.cpp
    ../BitmapDownsampled.cpp
    ../Debug.cpp 
)

add_definitions(-s -O3 -DNO_GZIP -DPNG_INTEL_SSE -march=native)
add_library(ttc STATIC ${SOURCES})

find_library(Libpng NAMES png)

target_link_libraries(ttc PUBLIC png)
set_property(TARGET ttc PROPERTY C_STANDARD 99)

target_include_directories(ttc
    INTERFACE
        include
    PRIVATE
        src
)