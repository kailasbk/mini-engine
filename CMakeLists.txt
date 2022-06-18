cmake_minimum_required(VERSION 3.22)
project(opengl_renderer C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

include_directories(include)
link_directories(lib)

add_executable(opengl_renderer main.cpp src/VertexBuffer.cpp src/VertexBuffer.h src/IndexBuffer.cpp src/IndexBuffer.h src/Shader.cpp src/Shader.h src/Program.cpp src/Program.h src/VertexFormat.cpp src/VertexFormat.h src/Math.cpp src/Math.h src/Camera.h src/Camera.cpp src/Grid.cpp src/Grid.h src/ObjLoader.cpp src/ObjLoader.h)

target_link_libraries(opengl_renderer mingw32 SDL2main SDL2 glew32 opengl32)