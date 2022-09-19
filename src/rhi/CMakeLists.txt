target_sources(engine PRIVATE
        RHI.cpp RHI.h Buffer.h Texture2D.h Shader.h Pipeline.h
        Framebuffer.h VertexLayout.h Format.h Resource.h Uniform.cpp Uniform.h DescriptorSet.h UniformVisitor.h)

add_subdirectory(opengl)