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
    v2f_color = bundle.vertexes[gl_VertexID].BBColor;
    v2f_texcoord = bundle.vertexes[gl_VertexID].BBTexcoord.xy;
    gl_Position = BBProjectionMatrix * BBViewMatrix * BBModelMatrix * bundle.vertexes[gl_VertexID].BBPosition;
}
