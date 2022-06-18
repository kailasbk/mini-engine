#include "Program.h"

#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

Program::Program()
        : m_programID(glCreateProgram()), m_shaders() {}

Program::~Program() {
    glDeleteProgram(m_programID);
}

void Program::addShader(const std::shared_ptr<Shader>& shader, Shader::Type stage) {
    if (shader->getType() == stage) {
        // detach the old shader
        if (m_shaders.count(stage) != 0) {
            glDetachShader(m_programID, m_shaders[stage]->getID());
        }

        // attach the new shader
        glAttachShader(m_programID, shader->getID());
        m_shaders[stage] = shader;
    }
}

void Program::setVec3(const std::string& name, const glm::vec3& vector) const {
    int location = glGetUniformLocation(m_programID, name.c_str());
    glUniform3fv(location, 1, glm::value_ptr(vector));
}

void Program::setMat4(const std::string& name, const glm::mat4& matrix) const {
    int location = glGetUniformLocation(m_programID, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Program::link() const {
    glLinkProgram(m_programID);

    int success;
    glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
    if (success != GL_TRUE) {
        std::cerr << "ERR: program failed to link." << std::endl;
    }
}

void Program::bind() const {
    glUseProgram(m_programID);
}
