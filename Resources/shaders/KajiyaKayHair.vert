#version 330 core

in vec4 BBPosition;
in vec4 BBTexcoord;
in vec4 BBNormal;

out vec2 v2f_texcoords;
out vec3 v2f_world_pos;
out vec3 v2f_normal;
out vec3 v2f_view_dir;

uniform mat4 BBProjectionMatrix;
uniform mat4 BBViewMatrix;
uniform mat4 BBModelMatrix;
uniform vec4 BBCameraPosition;

void main()
{
    vec4 world_pos = BBModelMatrix * BBPosition;
    gl_Position = BBProjectionMatrix * BBViewMatrix * world_pos;

    v2f_texcoords = BBTexcoord.xy;
    v2f_world_pos = world_pos.xyz;
    v2f_normal = mat3(BBModelMatrix) * BBNormal.xyz;
    v2f_view_dir = BBCameraPosition.xyz - v2f_world_pos;
}
