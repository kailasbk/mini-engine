#ifndef OPENGL_RENDERER_FRAMEBUFFER_H
#define OPENGL_RENDERER_FRAMEBUFFER_H

#include "Texture2D.h"

class Framebuffer {
public:
    /**
     * Constructs the framebuffer with the given dimensions, which starts without attachments.
     *
     * @param width the width of the framebuffer, in pixels
     * @param height the height of the framebuffer, in pixels
     */
    Framebuffer(int width, int height);

    /**
     * Destructs the framebuffer.
     */
    ~Framebuffer();

    /**
     * Adds color attachment to the framebuffer from the given texture.
     *
     * @param texture the 2d texture to attach
     */
    void addColorAttachment(const std::shared_ptr<Texture2D>& texture);

    /**
     * Adds a depth attachment to the framebuffer.
     */
    void addDepthAttachment();

    /**
     * Copies the framebuffer color attachments from the source framebuffer, resolving sampling if needed.
     *
     * @param source the source framebuffer
     */
    void copyColorAttachments(const Framebuffer& source) const;

    /**
     * Binds the frame buffer to be drawn to.
     */
    void bind() const;

    /**
     * Unbinds the bound framebuffer, binding the default framebuffer.
     */
    static void unbind();

    /**
     * @return the id of the framebuffer
     */
    inline unsigned int getID() const {
        return m_framebufferID;
    }

private:
    int m_width, m_height;
    unsigned int m_framebufferID;
    std::vector<std::shared_ptr<const Texture2D>> m_colorAttachments;
    unsigned int m_depthID;
    int m_numSamples;
};


#endif //OPENGL_RENDERER_FRAMEBUFFER_H
