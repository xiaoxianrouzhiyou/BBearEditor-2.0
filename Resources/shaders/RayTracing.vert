#version 430 core

in vec4 BBPosition;
in vec4 BBTexcoord;
in vec4 BBNormal;

out vec4 v2f_world_space_pos;
out vec2 v2f_texcoord;
out vec4 v2f_normal;

uniform mat4 BBProjectionMatrix;
uniform mat4 BBViewMatrix;
uniform mat4 BBModelMatrix;

void main()
{
    v2f_texcoord = BBTexcoord.xy;
    v2f_normal = transpose(inverse(BBModelMatrix)) * BBNormal;
    v2f_normal = normalize(v2f_normal);
    v2f_world_space_pos = BBModelMatrix * BBPosition;
    gl_Position = BBProjectionMatrix * BBViewMatrix * v2f_world_space_pos;
}
