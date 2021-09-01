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
    vec3 world_pos;
	vec4 color;
	vec3 normal;
} v2g;

uniform mat4 BBProjectionMatrix;
uniform mat4 BBViewMatrix;
uniform mat4 BBModelMatrix;

void main()
{
    v2g.color = bundle.vertexes[gl_VertexID].BBColor;
    v2g.normal = mat3(transpose(inverse(BBModelMatrix))) * bundle.vertexes[gl_VertexID].BBNormal.xyz;
    vec4 world_pos = BBModelMatrix * bundle.vertexes[gl_VertexID].BBPosition;
    v2g.world_pos = world_pos.xyz;
    gl_Position = BBProjectionMatrix * BBViewMatrix * world_pos;
}
