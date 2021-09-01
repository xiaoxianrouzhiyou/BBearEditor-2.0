#version 430 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 12) out;

in V2G
{
    vec3 world_pos;
	vec4 color;
	vec3 normal;
} v2g[];

out G2F
{
	vec4 color;
    vec3 normal;
} g2f;

void main()
{
    g2f.color = v2g[0].color;
    g2f.normal = v2g[0].normal;
    gl_Position = vec4(v2g[0].world_pos, 1.0);
    EmitVertex();

    g2f.color = v2g[1].color;
    g2f.normal = v2g[1].normal;
    gl_Position = vec4(v2g[1].world_pos, 1.0);
    EmitVertex();

    g2f.color = v2g[2].color;
    g2f.normal = v2g[2].normal;
    gl_Position = vec4(v2g[2].world_pos, 1.0);
    EmitVertex();

    g2f.color = v2g[2].color;
    g2f.normal = v2g[2].normal;
    gl_Position = vec4(0.0, 0.0, 0.0, 1.0);
    EmitVertex();

    EndPrimitive();
}