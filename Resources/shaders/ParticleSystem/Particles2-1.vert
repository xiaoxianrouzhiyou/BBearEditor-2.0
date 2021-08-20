#version 430

attribute vec4 BBPosition;

uniform mat4 BBProjectionMatrix;
uniform mat4 BBViewMatrix;

void main()
{
    gl_Position = BBProjectionMatrix * BBViewMatrix * BBPosition;
}
