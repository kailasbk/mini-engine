cmake_minimum_required(VERSION 3.22)
project(engine C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

include_directories(include)
link_directories(lib)

add_executable(engine)
target_sources(engine PRIVATE main.cpp)
target_precompile_headers(engine PRIVATE pch.h)
target_link_libraries(engine SDL2main SDL2 glew32 opengl32)

add_subdirectory(src)
