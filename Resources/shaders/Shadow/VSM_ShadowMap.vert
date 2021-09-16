#version 430 core

in vec4 BBPosition;

uniform mat4 BBLightProjectionMatrix;
uniform mat4 BBLightViewMatrix;
uniform mat4 BBModelMatrix;

void main()
{
	gl_Position = BBLightProjectionMatrix * BBLightViewMatrix * BBModelMatrix * BBPosition;
}
