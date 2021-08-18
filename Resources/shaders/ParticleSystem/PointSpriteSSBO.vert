#version 430

struct Vertex
{
    vec4 BBPosition;
    vec4 BBColor;
    vec4 BBTexcoord;
};

layout (std140, binding = 0) buffer Bundle
{
    Vertex vertexes[];
} bundle;

uniform mat4 BBProjectionMatrix;
uniform mat4 BBViewMatrix;
uniform mat4 BBModelMatrix;

varying vec2 v2f_texcoord;

void main()
{
    v2f_texcoord = bundle.vertexes[gl_VertexID].BBTexcoord.xy;
    gl_Position = BBProjectionMatrix * BBViewMatrix * BBModelMatrix * bundle.vertexes[gl_VertexID].BBPosition;
}
