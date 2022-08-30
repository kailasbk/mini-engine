#include "OpenGLFormat.h"

GLenum toOpenGLFormat(Format format) {
    switch (format) {
        case Format::RGB8: return GL_RGB8;
        case Format::RGBA8: return GL_RGBA8;
        case Format::RG32F: return GL_RG32F;
        case Format::RGB32F: return GL_RGB32F;
        case Format::RGBA32F: return GL_RGBA32F;
        case Format::D32F: return GL_DEPTH_COMPONENT32F;
        default: throw std::invalid_argument("OpenGL does not support that format.");
    }
}
