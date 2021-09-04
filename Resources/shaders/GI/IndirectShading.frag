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

uniform float S[64];
uniform sampler2D AlbedoTex;
uniform sampler2D NormalTex;
uniform sampler2D PositionTex;

void main(void)
{
	int VPL_num = int(atomicCounter(TriangleID));
    FragColor = vec4(VPL_num / 10000000, 0.0, 0.0, 1.0);
}
