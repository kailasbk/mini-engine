#include "ShaderLoader.h"
#include <fstream>
#include <sstream>

std::unique_ptr<Shader> shaderFromFile(const std::string& filename) {
    ShaderType type = ShaderType::Vertex;
    if (filename.find(".vert") != std::string::npos) {
        type = ShaderType::Vertex;
    } else if (filename.find(".frag") != std::string::npos) {
        type = ShaderType::Fragment;
    }

    std::fstream file;
    file.open(filename, std::fstream::in);
    if (!file.good()) {
        std::cerr << "ERR: failed to load file: " << filename << std::endl;
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    return defaultRenderAPI()->createShader((void*)buffer.str().c_str(), buffer.str().size(), type);
}
