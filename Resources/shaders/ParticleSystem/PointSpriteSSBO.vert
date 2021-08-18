#version 430

struct Vertex
{
    vec4 BBPosition;
    vec4 BBColor;
    vec4 BBTexcoord;
    vec4 BBNormal;
    vec4 BBTangent;
    vec4 BBBiTangent;
};

layout (std140, binding = 0) buffer Bundle
{
    Vertex vertexes[];
} bundle;

uniform mat4 BBProjectionMatrix;
uniform mat4 BBViewMatrix;
uniform mat4 BBModelMatrix;

varying vec4 v2f_color;
varying vec2 v2f_texcoord;

void main()
{
    // Every four vertices form a point sprite
    // >> 2 : / 4
    int sprite_id = gl_VertexID >> 2;
    vec4 sprite_pos = bundle.vertexes[sprite_id].BBPosition;
    // point sprite needs to face us
    vec4 sprite_pos_view_space = BBViewMatrix * BBModelMatrix * sprite_pos;
    v2f_texcoord = vec2(((gl_VertexID - 1) & 2) >> 1, (gl_VertexID & 2) >> 1);
    float sprite_size = 0.015;
    // Use one point coordinate to calculate four vertex coordinates
    vec3 offset = vec3((v2f_texcoord * 2.0 - 1.0) * sprite_size, 0.0);
    vec4 fixed_pos = vec4(sprite_pos_view_space.xyz + offset, 1.0);

    v2f_color = bundle.vertexes[gl_VertexID].BBColor;
    gl_Position = BBProjectionMatrix * fixed_pos;
}
