target_sources(engine PRIVATE
        TextureLoader.cpp TextureLoader.h
        StaticMesh.cpp StaticMesh.h
        Material.cpp Material.h
        Camera3D.cpp Camera3D.h
        Renderer3D.cpp Renderer3D.h
        Grid.cpp Grid.h
        ObjLoader.cpp ObjLoader.h
        StaticMeshLoader.cpp StaticMeshLoader.h
        ShaderLoader.cpp ShaderLoader.h
        ecs/Entity.h
        ecs/System.h
        ecs/Scene.h
        ecs/View.h
        ecs/SparseSet.h
        )