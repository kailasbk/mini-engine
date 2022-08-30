#include "OpenGLTexture2D.h"
#include "OpenGLFormat.h"

std::unique_ptr<Texture2D> OpenGLRHI::createTexture2D(Format format, uint32_t width, uint32_t height) {
    GLuint handle;
    glCreateTextures(GL_TEXTURE_2D, 1, &handle);

    // set texture parameters
    glTextureParameteri(handle, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(handle, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // allocate the texture storage
    glTextureStorage2D(handle, 1, toOpenGLFormat(format), (GLsizei)width, (GLsizei)height);

    return std::make_unique<OpenGLTexture2D>(handle, format, width, height);
}
