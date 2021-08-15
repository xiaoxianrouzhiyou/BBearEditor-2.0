#version 430 core

in vec4 BBPosition;
in vec4 BBTexcoord;

out vec3 v2f_world_space_pos;
out vec2 v2f_texcoord;

uniform mat4 BBProjectionMatrix;
uniform mat4 BBViewMatrix;
uniform mat4 BBModelMatrix;

void main()
{
    v2f_texcoord = BBTexcoord.xy;
    vec4 world_space_pos = BBModelMatrix * BBPosition;
    gl_Position = BBProjectionMatrix * BBViewMatrix * world_space_pos;
    v2f_world_space_pos = world_space_pos.xyz;
}
