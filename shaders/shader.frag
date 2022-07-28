#version 460 core

in vec3 fPosition;
in vec2 fTexCoord;
in vec3 fNormal;

out vec3 oColor;

uniform vec3 LightPosition;
layout(binding = 0) uniform sampler2D AlbedoTexture;

void main()
{
    vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
    //vec3 albedo = texture(AlbedoTexture, fTexCoord).rgb;
    vec3 albedo = vec3(0.6f, 0.6f, 0.6f);

    float ambientLevel = 0.1f;

    vec3 lightDir = normalize(LightPosition - fPosition);
    vec3 normalDir = normalize(fNormal);
    float diffuseLevel = max(dot(normalDir, lightDir), 0.0f);

    oColor = albedo * lightColor * (ambientLevel + diffuseLevel);
}
