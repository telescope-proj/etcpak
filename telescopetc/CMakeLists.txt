include_directories(include)
set(SOURCES
    src/ttc_wrapper.cpp
    ../ProcessDxtc.cpp
)

add_definitions(-s -O3 -Wall -DNO_GZIP -DPNG_INTEL_SSE -march=native)
add_library(ttc STATIC ${SOURCES})

set_property(TARGET ttc PROPERTY C_STANDARD 99)

target_include_directories(ttc
    INTERFACE
        include
    PRIVATE
        src
)