#version 460 core

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec2 vTexCoord;
layout(location = 2) in vec3 vNormal;

out vec3 fPosition;
out vec2 fTexCoord;
out vec3 fNormal;

layout(location = 0) uniform mat4 MVP;

void main()
{
    fPosition = vPosition;
    fTexCoord = vTexCoord;
    fNormal = vNormal;
    gl_Position = MVP * vec4(vPosition, 1.0f);
}
