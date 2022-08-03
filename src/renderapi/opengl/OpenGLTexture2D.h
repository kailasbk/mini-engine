#ifndef OPENGL_RENDERER_OPENGLTEXTURE2D_H
#define OPENGL_RENDERER_OPENGLTEXTURE2D_H

#include "OpenGLAPI.h"

class OpenGLTexture2D : public Texture2D, public Resource<GLuint> {
public:
    OpenGLTexture2D(GLuint handle, Format format, uint32_t width, uint32_t height)
            : Resource<GLuint>(handle), Texture2D(format, width, height) {}

    ~OpenGLTexture2D() override {
        glDeleteTextures(1, &m_handle);
    }

    constexpr static OpenGLTexture2D& from(Texture2D& texture2D) {
        return dynamic_cast<OpenGLTexture2D&>(texture2D);
    }

};


#endif //OPENGL_RENDERER_OPENGLTEXTURE2D_H
