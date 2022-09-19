#ifndef OPENGL_RENDERER_OPENGLRHI_H
#define OPENGL_RENDERER_OPENGLRHI_H

#include "../RHI.h"
#include "../Resource.h"

class OpenGLRHI : public RHI {
public:
    OpenGLRHI() : m_vertexArray(0), m_binds{nullptr, nullptr} {
        // load opengl pointers from glew
        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK) {
            throw std::runtime_error("Failed to load OpenGL.");
        }

        // enable depth test and multisampling
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_MULTISAMPLE);

        // uses only one vertex array, changing its values
        glCreateVertexArrays(1, &m_vertexArray);
        glBindVertexArray(m_vertexArray);

        // use [0, 1] z coords for NDCs
        glClipControl(GL_LOWER_LEFT, GL_ZERO_TO_ONE);
    }

    ~OpenGLRHI() override {
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
    void bindUniforms(UniformBlock& uniformBlock) override;
    void bindDescriptorSet(const DescriptorSet& descriptorSet) override;
    void bindPipeline(const Pipeline& pipeline) override;
    void bindFramebuffer(Framebuffer& framebuffer) override;
    void bindDefaultFramebuffer() override;

    void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
    void clearAttachments(float r, float g, float b, float a, float depth) override;

    void draw(uint32_t vertexCount, uint32_t baseVertex) override;
    void drawIndexed(uint32_t indexCount, uint32_t baseIndex, uint32_t baseVertex) override;

private:
    struct {
        const Buffer* indexBuffer;
        const Pipeline* pipeline;
    } m_binds;
    GLuint m_vertexArray;
};


#endif //OPENGL_RENDERER_OPENGLRHI_H
