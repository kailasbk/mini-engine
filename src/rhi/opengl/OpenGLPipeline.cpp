#include "OpenGLPipeline.h"

std::unique_ptr<PipelineBuilder> OpenGLRHI::createPipelineBuilder() {
    return std::make_unique<OpenGLPipelineBuilder>();
}

std::unique_ptr<Pipeline> OpenGLPipelineBuilder::build() {
    GLuint handle = glCreateProgram();

    glAttachShader(handle, m_vertexShader->handle());
    glAttachShader(handle, m_fragmentShader->handle());
    glLinkProgram(handle);

    GLint success;
    glGetProgramiv(handle, GL_LINK_STATUS, &success);
    if (success != GL_TRUE) {
        // TODO: update to use exceptions
        std::cerr << "ERR: program failed to link." << std::endl;
    }

    glDetachShader(handle, m_vertexShader->handle());
    glDetachShader(handle, m_fragmentShader->handle());

    return std::make_unique<OpenGLPipeline>(handle, m_topology, *m_vertexLayout);
}
