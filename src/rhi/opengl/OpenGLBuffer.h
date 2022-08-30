#ifndef OPENGL_RENDERER_OPENGLBUFFER_H
#define OPENGL_RENDERER_OPENGLBUFFER_H

#include "OpenGLRHI.h"

class OpenGLBuffer : public Buffer, public Resource<GLuint> {
public:
    explicit OpenGLBuffer(GLuint handle, uint32_t size, uint32_t stride)
            : Buffer(size, stride), Resource<GLuint>(handle), m_isMapped(false) {}

    ~OpenGLBuffer() override {
        glDeleteBuffers(1, &m_handle);
    }

    void* map(uint32_t offset, uint32_t size) override;
    void unmap() override;
    bool isMapped() const override;

    constexpr static OpenGLBuffer& from(Buffer& buffer) {
        return dynamic_cast<OpenGLBuffer&>(buffer);
    }

    constexpr static const OpenGLBuffer& from(const Buffer& buffer) {
        return dynamic_cast<const OpenGLBuffer&>(buffer);
    }

    static constexpr GLbitfield mapFlags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
private:
    bool m_isMapped;
};


#endif //OPENGL_RENDERER_OPENGLBUFFER_H
