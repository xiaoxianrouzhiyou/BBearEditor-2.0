#version 330 core

in vec4 BBPosition;

out vec3 v2f_local_pos;

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;

void main()
{
    v2f_local_pos = BBPosition.xyz;
    gl_Position = ProjectionMatrix * ViewMatrix * BBPosition;
}
