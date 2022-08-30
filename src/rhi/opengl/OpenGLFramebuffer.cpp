#include "OpenGLFramebuffer.h"
#include "OpenGLTexture2D.h"

std::unique_ptr<FramebufferBuilder> OpenGLRHI::createFramebufferBuilder() {
    return std::make_unique<OpenGLFramebufferBuilder>();
}

std::unique_ptr<Framebuffer> OpenGLFramebufferBuilder::build() {
    assert(m_colorAttachment != nullptr); // must have a color attachment
    assert(m_width > 0 && m_height > 0); // must have positive dimensions

    GLuint handle;
    glCreateFramebuffers(1, &handle);

    glNamedFramebufferTexture(handle, GL_COLOR_ATTACHMENT0, OpenGLTexture2D::from(*m_colorAttachment).handle(), 0);
    glNamedFramebufferDrawBuffer(handle, GL_COLOR_ATTACHMENT0);
    if (m_depthAttachment != nullptr) {
        glNamedFramebufferTexture(handle, GL_DEPTH_ATTACHMENT, OpenGLTexture2D::from(*m_depthAttachment).handle(), 0);
    }

    return std::make_unique<OpenGLFramebuffer>(handle, m_width, m_height, std::move(m_colorAttachment),
                                               std::move(m_depthAttachment));
}
