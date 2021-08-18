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

layout (local_size_x = 256, local_size_y = 1, local_size_z = 1) in;