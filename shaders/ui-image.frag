#version 460 core

in vec2 fTexCoord;

out vec3 oColor;

layout(binding = 0) uniform sampler2D Texture;

void main()
{
    oColor = texture(Texture, fTexCoord).rgb;
}
