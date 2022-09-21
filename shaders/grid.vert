#version 460 core

layout(location = 0) in vec3 iPos;

uniform mat4 MVP;

void main()
{
    gl_Position = MVP * vec4(iPos, 1.0f);
}
