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

out V2F
{
	vec4 color;
	vec3 normal;
} v2f;

uniform mat4 BBProjectionMatrix;
uniform mat4 BBViewMatrix;
uniform mat4 BBModelMatrix;

void main()
{
    v2f.color = bundle.vertexes[gl_VertexID].BBColor;
    v2f.normal = mat3(transpose(inverse(BBModelMatrix))) * bundle.vertexes[gl_VertexID].BBNormal.xyz;
    gl_Position = BBProjectionMatrix * BBViewMatrix * BBModelMatrix * bundle.vertexes[gl_VertexID].BBPosition;
}
