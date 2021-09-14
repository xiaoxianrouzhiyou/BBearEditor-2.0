#version 430 core

in vec4 BBPosition;

uniform mat4 BBProjectionMatrix;
uniform mat4 BBViewMatrix;
uniform mat4 BBModelMatrix;

void main()
{
	gl_Position = BBProjectionMatrix * BBViewMatrix * BBModelMatrix * BBPosition;
}
