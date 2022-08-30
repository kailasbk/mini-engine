#include "OpenGLShader.h"

std::unique_ptr<Shader> OpenGLRHI::createShader(const void* code, size_t codeSize, ShaderType type) {
    GLenum glType;
    switch (type) {
        case ShaderType::Vertex:
            glType = GL_VERTEX_SHADER;
            break;
        case ShaderType::Fragment:
            glType = GL_FRAGMENT_SHADER;
            break;
        case ShaderType::Compute:
            glType = GL_COMPUTE_SHADER;
    }
    GLuint handle = glCreateShader(glType);
    const char* str = (const char*)code;
    auto size = (GLint)codeSize;
    glShaderSource(handle, 1, &str, &size);
    glCompileShader(handle);

    GLint success;
    glGetShaderiv(handle, GL_COMPILE_STATUS, &success);
    if (success != GL_TRUE) {
        // TODO: make this an exception in the future
        std::cerr << "ERR: shader failed to compile." << std::endl;
    }
    return std::make_unique<OpenGLShader>(handle, type);
}
