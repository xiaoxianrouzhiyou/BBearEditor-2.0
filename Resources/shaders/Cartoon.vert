#version 330 core

in vec4 BBPosition;
in vec4 BBNormal;

out vec3 v2f_world_pos;
out vec3 v2f_normal;

uniform mat4 BBProjectionMatrix;
uniform mat4 BBViewMatrix;
uniform mat4 BBModelMatrix;

void main()
{
    vec4 world_pos = BBModelMatrix * BBPosition;
    gl_Position = BBProjectionMatrix * BBViewMatrix * world_pos;

    v2f_world_pos = world_pos.xyz;
    v2f_normal = mat3(BBModelMatrix) * BBNormal.xyz;
}
