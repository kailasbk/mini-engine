target_sources(opengl_renderer PRIVATE
        RenderAPI.cpp RenderAPI.h Buffer.h Texture2D.h Shader.h Pipeline.h
        Framebuffer.h VertexLayout.h Format.h Resource.h Uniform.cpp Uniform.h DescriptorSet.h UniformVisitor.h)

add_subdirectory(opengl)