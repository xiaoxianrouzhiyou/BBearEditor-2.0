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

out V2G
{
    vec3 position;
    vec3 normal;
    vec2 texcoord;
} v2g;

uniform mat4 BBModelMatrix;

void main()
{
    v2g.position = (BBModelMatrix * bundle.vertexes[gl_VertexID].BBPosition).xyz;
    v2g.normal = bundle.vertexes[gl_VertexID].BBNormal.xyz;
    v2g.texcoord = bundle.vertexes[gl_VertexID].BBTexcoord.xy;
    gl_Position = vec4(0.0, 0.0, 0.0, 1.0);
}
