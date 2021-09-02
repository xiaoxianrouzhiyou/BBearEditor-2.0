#version 430 core

struct G2FTriangle
{
    vec3 positions[3];
    vec3 normal;
    vec2 texcoord;
    vec2 area_and_level;
};

in G2F
{
    G2FTriangle triangle;
} g2f;

layout (location = 0) out vec4 FragColor;

struct TriangleCut
{
	vec4 center;
	vec4 normal_and_level;
	vec4 color_and_area;
};
layout (std430, binding = 0) buffer Triangles
{
	TriangleCut triangles[];
};

layout (binding = 0, offset = 0) uniform atomic_uint TriangleID;

void main(void)
{
    FragColor = vec4(1.0, 0.0, 0.0, 1.0);

    vec3 center = (g2f.triangle.positions[0] + g2f.triangle.positions[1] + g2f.triangle.positions[2]) / 3.0;

    int current_id = int(atomicCounterIncrement(TriangleID));
    triangles[current_id].center = vec4(center, 1.0);
    triangles[current_id].normal_and_level = vec4(g2f.triangle.normal, g2f.triangle.area_and_level.y);
    triangles[current_id].color_and_area = vec4(1.0, 0.0, 0.0, g2f.triangle.area_and_level.x);
}

