include_directories(include)
set(SOURCES
    src/ttc_wrapper.cpp
    src/mmap.cpp
)

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