cmake_minimum_required(VERSION 3.22)
project(opengl_renderer C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

include_directories(include)
link_directories(lib)

add_executable(opengl_renderer main.cpp
        src/Camera3D.cpp src/Camera3D.h
        src/Grid.cpp src/Grid.h
        src/ObjLoader.cpp src/ObjLoader.h
        src/Window.cpp src/Window.h
        )

add_subdirectory(src/util)
add_subdirectory(src/renderapi)

target_precompile_headers(opengl_renderer PRIVATE pch.h)
target_link_libraries(opengl_renderer SDL2main SDL2 glew32 opengl32)