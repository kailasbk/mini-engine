#ifndef OPENGL_RENDERER_SHADERLOADER_H
#define OPENGL_RENDERER_SHADERLOADER_H

#include "../renderapi/RenderAPI.h"

std::unique_ptr<Shader> shaderFromFile(const std::string& filename);


#endif //OPENGL_RENDERER_SHADERLOADER_H
