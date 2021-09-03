#version 430 core

in vec2 v2f_texcoord;

out vec4 FragColor;

struct TriangleCut
{
	vec4 center;
	vec4 normal_and_level;
	vec4 color_and_area;
};
layout (std430, binding = 1) buffer Triangles
{
	TriangleCut triangles[];
};

layout (binding = 0, offset = 0) uniform atomic_uint TriangleID;

uniform sampler2D AlbedoTex;

void main(void)
{
	int VPL_num = int(atomicCounter(TriangleID));
    FragColor = vec4(VPL_num, 0.0, 0.0, 1.0);
}
