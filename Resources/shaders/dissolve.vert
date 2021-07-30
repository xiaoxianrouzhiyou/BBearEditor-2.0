#version 450 core

in vec4 BBPosition;
in vec4 BBTexcoord;

out vec4 V_Texcoord;

uniform mat4 BBProjectionMatrix;
uniform mat4 BBViewMatrix;
uniform mat4 BBModelMatrix;

void main()
{
    V_Texcoord = BBTexcoord;

    gl_Position = BBProjectionMatrix * BBViewMatrix * BBModelMatrix * BBPosition;
}
