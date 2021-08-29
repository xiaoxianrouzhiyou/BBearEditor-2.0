#version 330 core

in vec4 BBPosition;

out vec3 v2f_local_pos;

uniform mat4 BBProjectionMatrix;
uniform mat4 BBViewMatrix;
uniform mat4 BBModelMatrix;

void main()
{
    v2f_local_pos = BBPosition.xyz;
    gl_Position = BBProjectionMatrix * BBViewMatrix * BBModelMatrix * BBPosition;
}
