#version 430 core

in vec2 v2f_texcoord;

layout (location = 0) out vec4 FragColor;

uniform sampler2D NormalTex;
uniform sampler2D PositionTex;

void main(void)
{
    FragColor = texture(PositionTex, v2f_texcoord);
}

