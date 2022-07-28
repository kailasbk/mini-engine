#include "Shader.h"

#include <sstream>
#include <fstream>

std::shared_ptr<Shader> Shader::fromFile(const std::string& filename) {
    Type type = Type::Undefined;
    if (filename.find(".vert") != std::string::npos) {
        type = Type::Vertex;
    } else if (filename.find(".frag") != std::string::npos) {
        type = Type::Fragment;
    }

    std::fstream file;
    file.open(filename, std::fstream::in);
    if (!file.good()) {
        std::cerr << "ERR: failed to load file: " << filename << std::endl;
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    return std::make_shared<Shader>(buffer.str().c_str(), type);
}

Shader::Shader(const char* source, Shader::Type type)
        : m_type(type), m_shaderID(0) {
    GLenum glType;
    switch (m_type) {
        case Type::Vertex:
            glType = GL_VERTEX_SHADER;
            break;
        case Type::Fragment:
            glType = GL_FRAGMENT_SHADER;
            break;
        case Type::Undefined:
            std::cerr << "ERR: shader of undefined type loaded." << std::endl;
            return;
    }
    m_shaderID = glCreateShader(glType);
    glShaderSource(m_shaderID, 1, &source, nullptr);
    glCompileShader(m_shaderID);

    int success;
    glGetShaderiv(m_shaderID, GL_COMPILE_STATUS, &success);
    if (success != GL_TRUE) {
        std::cerr << "ERR: shader failed to compile." << std::endl;
    }
}

Shader::~Shader() {
    glDeleteShader(m_shaderID);
}

unsigned int Shader::getID() const {
    return m_shaderID;
}

Shader::Type Shader::getType() const {
    return m_type;
}
