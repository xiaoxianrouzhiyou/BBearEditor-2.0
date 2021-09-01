#version 430 core

in vec2 v2f_texcoord;

layout (location = 0) out vec4 FragColor;

uniform sampler2D AlbedoTex;

void main(void)
{
    FragColor = texture(AlbedoTex, v2f_texcoord);
}
