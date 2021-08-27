#version 430 core

in vec4 BBPosition;
in vec4 BBTexcoord;

out vec2 v2f_texcoord;

void main()
{
    v2f_texcoord = BBTexcoord.xy;
    gl_Position = BBPosition;
}
