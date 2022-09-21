#version 460 core

in vec2 fTexCoord;

out vec4 oColor;

layout(location = 1) uniform vec4 Color;

void main()
{
    oColor = Color;
}
