#version 430 core

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

out vec2 v2f_texcoord;


void main()
{
    v2f_texcoord = bundle.vertexes[gl_VertexID].BBTexcoord.xy;
    gl_Position = bundle.vertexes[gl_VertexID].BBPosition;
}
