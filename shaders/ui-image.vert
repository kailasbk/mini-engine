#version 460 core

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec2 vTexCoord;

layout(location = 0) uniform mat4 Projection;

out vec2 fTexCoord;

void main()
{
    fTexCoord = vTexCoord;
    gl_Position = Projection * vec4(vPosition, 1.0f);
}
