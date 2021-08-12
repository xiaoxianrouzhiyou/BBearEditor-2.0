#version 430 core
#extension GL_NV_shadow_samplers_cube : enable

in vec4 v2f_local_pos;

layout (location = 0) out vec4 FragColor;

uniform samplerCube BBSkyBox;

void main(void)
{
    FragColor = textureCube(BBSkyBox, v2f_local_pos.xyz);
}
