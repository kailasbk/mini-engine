#ifndef OPENGL_RENDERER_FRAMEBUFFER_H
#define OPENGL_RENDERER_FRAMEBUFFER_H


class Framebuffer {
public:
    Framebuffer(uint32_t width, uint32_t height, std::unique_ptr<Texture2D> colorAttachment,
                std::unique_ptr<Texture2D> depthAttachment)
            : m_width(width), m_height(height), m_colorAttachment(std::move(colorAttachment)),
              m_depthAttachment(std::move(depthAttachment)) {}

    virtual ~Framebuffer() = default;

    /**
     * @returns the 2d dimensions of the framebuffer
     */
    Vector<uint32_t, 2> dimensions() const {
        return {m_width, m_height};
    };

    /**
     * @returns a reference to the framebuffer's color attachment
     */
    Texture2D& colorAttachment() const {
        return *m_colorAttachment;
    };

    /**
     * @returns a pointer to the framebuffer's depth attachment, which is null if none exists
     */
    Texture2D* depthAttachment() const {
        return m_depthAttachment.get();
    };

private:
    uint32_t m_width;
    uint32_t m_height;
    std::unique_ptr<Texture2D> m_colorAttachment; // TODO: consider using shared ptr, having it give a shared_ptr back
    std::unique_ptr<Texture2D> m_depthAttachment;
};

class FramebufferBuilder {
public:
    virtual ~FramebufferBuilder() = default;

    virtual FramebufferBuilder* setDimensions(uint32_t width, uint32_t height) = 0;

    virtual FramebufferBuilder* setColorAttachment(std::unique_ptr<Texture2D> attachment) = 0;

    virtual FramebufferBuilder* setDepthAttachment(std::unique_ptr<Texture2D> attachment) = 0;

    virtual std::unique_ptr<Framebuffer> build() = 0;
};


#endif //OPENGL_RENDERER_FRAMEBUFFER_H
