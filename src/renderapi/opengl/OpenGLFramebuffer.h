#ifndef OPENGL_RENDERER_OPENGLFRAMEBUFFER_H
#define OPENGL_RENDERER_OPENGLFRAMEBUFFER_H

#include "OpenGLAPI.h"

class OpenGLFramebuffer : public Framebuffer, public Resource<GLuint> {
public:
    OpenGLFramebuffer(GLuint handle, uint32_t width, uint32_t height, std::unique_ptr<Texture2D> colorAttachment,
                      std::unique_ptr<Texture2D> depthAttachment)
            : Framebuffer(width, height, std::move(colorAttachment), std::move(depthAttachment)),
              Resource<GLuint>(handle) {};

    ~OpenGLFramebuffer() override {
        glDeleteFramebuffers(1, &m_handle);
    }

    constexpr static OpenGLFramebuffer& from(Framebuffer& framebuffer) {
        return dynamic_cast<OpenGLFramebuffer&>(framebuffer);
    }

};

class OpenGLFramebufferBuilder : public FramebufferBuilder {
public:
    OpenGLFramebufferBuilder() : m_width(0), m_height(0), m_colorAttachment(nullptr), m_depthAttachment(nullptr) {}

    FramebufferBuilder* setDimensions(uint32_t width, uint32_t height) override {
        m_width = width;
        m_height = height;
        return this;
    }

    FramebufferBuilder* setColorAttachment(std::unique_ptr<Texture2D> attachment) override {
        m_colorAttachment = std::move(attachment);
        return this;
    }

    FramebufferBuilder* setDepthAttachment(std::unique_ptr<Texture2D> attachment) override {
        m_depthAttachment = std::move(attachment);
        return this;
    }

    std::unique_ptr<Framebuffer> build() override;

private:
    uint32_t m_width;
    uint32_t m_height;
    std::unique_ptr<Texture2D> m_colorAttachment;
    std::unique_ptr<Texture2D> m_depthAttachment;
};


#endif //OPENGL_RENDERER_OPENGLFRAMEBUFFER_H
