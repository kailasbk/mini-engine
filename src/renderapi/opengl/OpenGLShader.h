#ifndef OPENGL_RENDERER_OPENGLSHADER_H
#define OPENGL_RENDERER_OPENGLSHADER_H

#include "OpenGLAPI.h"

class OpenGLShader : public Shader, public Resource<GLuint> {
public:
    OpenGLShader(GLuint handle, ShaderType type) : Resource<GLuint>(handle), Shader(type) {};

    ~OpenGLShader() override {
        glDeleteShader(m_handle);
    }

    constexpr static OpenGLShader& from(Shader& shader) {
        return dynamic_cast<OpenGLShader&>(shader);
    }
};


#endif //OPENGL_RENDERER_OPENGLSHADER_H
