#include "Framebuffer.h"

#include <GL/glew.h>

Framebuffer::Framebuffer(int width, int height)
        : m_width(width), m_height(height), m_numSamples(0), m_framebufferID(0), m_depthID(0), m_colorAttachments() {
    glCreateFramebuffers(1, &m_framebufferID);
}

Framebuffer::~Framebuffer() {
    // delete framebuffer object
    glDeleteFramebuffers(1, &m_framebufferID);

    // delete depth attachment, if exists
    if (m_depthID != 0) {
        glDeleteRenderbuffers(1, &m_depthID);
    }
}

void Framebuffer::addColorAttachment(const std::shared_ptr<Texture2D>& texture) {
    int textureSamples = texture->getNumSamples();
    if (textureSamples != m_numSamples) {
        if (m_colorAttachments.empty()) {
            // set framebuffer sampling if first color attachment
            m_numSamples = textureSamples;
        } else {
            // otherwise throw a runtime error
            throw std::runtime_error("texture sampling does not match");
        }
    }
    glNamedFramebufferTexture(m_framebufferID, GL_COLOR_ATTACHMENT0 + m_colorAttachments.size(), texture->getID(), 0);
    m_colorAttachments.push_back(texture);
}

void Framebuffer::addDepthAttachment() {
    glCreateRenderbuffers(1, &m_depthID);

    // allocate renderbuffer to match sampling of color attachments
    if (m_numSamples == 0) {
        glNamedRenderbufferStorage(m_depthID, GL_DEPTH_COMPONENT, m_width, m_height);
    } else {
        glNamedRenderbufferStorageMultisample(m_depthID, m_numSamples, GL_DEPTH_COMPONENT, m_width, m_height);
    }

    // attach to the framebuffer
    glNamedFramebufferRenderbuffer(m_framebufferID, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthID);
}

void Framebuffer::copyColorAttachments(const Framebuffer& source) const {
    glNamedFramebufferReadBuffer(source.m_framebufferID, GL_COLOR_ATTACHMENT0);
    glNamedFramebufferDrawBuffer(m_framebufferID, GL_COLOR_ATTACHMENT0);
    glBlitNamedFramebuffer(source.m_framebufferID, m_framebufferID,
                           0, 0, source.m_width, source.m_height,
                           0, 0, m_width, m_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

void Framebuffer::bind() const {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_framebufferID);
}

void Framebuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}