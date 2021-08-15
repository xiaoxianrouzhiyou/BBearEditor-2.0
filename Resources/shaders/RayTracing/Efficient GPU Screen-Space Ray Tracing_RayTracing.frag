#version 430 core

in vec2 v2f_texcoord;

layout (location = 0) out float FragColor;

void main(void)
{
    FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
