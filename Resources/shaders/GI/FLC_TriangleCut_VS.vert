#version 430 core

in vec4 BBPosition;
in vec4 BBNormal;
in vec4 BBTexcoord;

out V2G
{
    vec3 position;
    vec3 normal;
    vec2 texcoord;
} v2g;

uniform mat4 BBModelMatrix;

void main()
{
    v2g.position = (BBModelMatrix * BBPosition).xyz;
    v2g.normal = BBNormal.xyz;
    v2g.texcoord = BBTexcoord.xy;
    gl_Position = vec4(0.0, 0.0, 0.0, 1.0);
}
