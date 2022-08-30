target_sources(opengl_renderer PRIVATE
        engine/Camera3D.cpp engine/Camera3D.h
        Grid.cpp Grid.h
        ObjLoader.cpp ObjLoader.h
        Window.cpp Window.h
        Application.h)

add_subdirectory(util)
add_subdirectory(rhi)
add_subdirectory(ui)
