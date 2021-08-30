#version 330 core

in vec4 BBPosition;
in vec4 BBTexcoord;

out vec2 v2f_texcoords;

void main()
{
    v2f_texcoords = BBTexcoord.xy;
	gl_Position = BBPosition;
}