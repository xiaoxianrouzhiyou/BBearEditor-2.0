#version 430 core

in vec4 BBPosition;

out vec4 v2f_local_pos;

uniform mat4 BBProjectionMatrix;
uniform mat4 BBViewMatrix;
uniform mat4 BBModelMatrix;

void main()
{
    v2f_local_pos = BBPosition;
    v2f_local_pos.y = -v2f_local_pos.y;
    gl_Position = BBProjectionMatrix * BBViewMatrix * BBModelMatrix * BBPosition;
}
