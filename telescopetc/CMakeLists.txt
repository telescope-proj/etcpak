cmake_minimum_required(VERSION 3.20)
project(ttc)

add_executable(telescopetc main.c)
add_library(${PROJECT_NAME} STATIC wrapper.cpp)

find_library(Libpng NAMES png)

target_link_libraries(telescopetc PRIVATE ${PROJECT_NAME} png)
