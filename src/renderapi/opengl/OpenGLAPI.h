#ifndef OPENGL_RENDERER_OPENGLAPI_H
#define OPENGL_RENDERER_OPENGLAPI_H

#include "../RenderAPI.h"
#include "../Resource.h"

class OpenGLAPI : public RenderAPI {
public:
    OpenGLAPI() : m_vertexArray(0), m_binds({nullptr}) {
        // uses only one vertex array, changing its values
        glCreateVertexArrays(1, &m_vertexArray);
        glBindVertexArray(m_vertexArray);
    }

    ~OpenGLAPI() override {
        glDeleteVertexArrays(1, &m_vertexArray);
    }

    std::unique_ptr<Buffer> createBuffer(uint32_t size, uint32_t stride) override;
    std::unique_ptr<Texture2D> createTexture2D(Format format, uint32_t width, uint32_t height) override;
    std::unique_ptr<Shader> createShader(const void* code, size_t codeSize, ShaderType type) override;
    std::unique_ptr<DescriptorSet> createDescriptorSet(std::vector<DescriptorSetBinding> bindings) override;

    std::unique_ptr<PipelineBuilder> createPipelineBuilder() override;
    std::unique_ptr<FramebufferBuilder> createFramebufferBuilder() override;

    void copyBufferToTexture2D(Buffer& source, Texture2D& destination) override;
    void resolveTexture2D(Texture2D& source, Texture2D& destination) override;

    void bindVertexBuffer(const Buffer& buffer, uint32_t binding) override;
    void bindIndexBuffer(const Buffer& buffer) override;
    void bindUniforms(UniformBlock& uniforms) override;
    void bindDescriptors(DescriptorSet& descriptors) override;
    void bindPipeline(Pipeline& pipeline) override;
    void bindFramebuffer(Framebuffer& framebuffer) override;
    void bindDefaultFramebuffer() override;

    void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
    void clearAttachments(float r, float g, float b, float a, float depth) override;

    void draw(uint32_t vertexCount, uint32_t baseVertex) override;
    void drawIndexed(uint32_t indexCount, uint32_t baseIndex, uint32_t baseVertex) override;

private:
    struct {
        const Buffer* indexBuffer;
        Pipeline* pipeline;
    } m_binds;
    GLuint m_vertexArray;
};


#endif //OPENGL_RENDERER_OPENGLAPI_H