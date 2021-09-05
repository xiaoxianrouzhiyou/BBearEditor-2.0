#version 430 core

in vec2 v2f_texcoord;

out vec4 FragColor;

struct TriangleCut
{
	vec4 center;
	vec4 normal_and_level;
	vec4 color_and_area;
};
layout (std140, binding = 1) buffer Triangles
{
	TriangleCut triangles[];
};

layout (binding = 0, offset = 0) uniform atomic_uint TriangleID;

uniform float S[64];
uniform sampler2D AlbedoTex;
uniform sampler2D NormalTex;
uniform sampler2D PositionTex;
uniform sampler2D TriangleCutPassDebugTex;

const float PI = 3.1415926;


float getF(int k, float d)
{
	if (k == 0) 
		return 1;

	float DkMinus = sqrt(S[k - 1]);
	float Dk = sqrt(S[k]);
	float DkPlus = sqrt(S[k + 1]);

	if (d <= sqrt(S[0]))
		return 0;
	else if (d >= DkMinus && d <= Dk)
		return (d - DkMinus) / (Dk - DkMinus);
	else if (d >= Dk && d <= DkPlus)
		return (DkPlus - d) / (DkPlus - Dk);
	else
		return 0;
}

float getSumSF(int level, float d)
{
	float sum = 0.0;
	for (int i = 0; i <= level; i++)
	{
		sum += S[i] * getF(i, d);
	}
	return sum;
}


void main(void)
{
	int VPL_num = int(atomicCounter(TriangleID));
	VPL_num = 5000;
    // GBuffer
	vec3 color = texture(AlbedoTex, v2f_texcoord).xyz;
	vec3 normal = texture(NormalTex, v2f_texcoord).xyz;
	vec3 position = texture(PositionTex, v2f_texcoord).xyz;

	vec3 indirect_light = vec3(0.0);
	for (int i = 0; i < VPL_num; i++)
	{
		vec3 VPL_position = triangles[i].center.xyz;
		vec3 VPL_normal = normalize(triangles[i].normal_and_level.xyz);
		// L * P / PI
		vec3 VPL_radiance = triangles[i].color_and_area.xyz * color * 1.5 / PI;
		vec3 w = position - VPL_position;
		float d2 = dot(w, w);
		int level = int(triangles[i].normal_and_level.w);
		float d = sqrt(d2);
		// SumSF * H
		// indirect_light += getSumSF(level, d) * VPL_radiance * max(dot(VPL_normal, w), 0.0) * max(dot(normal, -w), 0.0) / d2;
		indirect_light += getSumSF(1, d) * VPL_radiance * max(dot(VPL_normal, w), 0.0) * max(dot(normal, -w), 0.0) / d2;
	}

	FragColor = vec4(indirect_light, 1.0);
	// FragColor = texture(TriangleCutPassDebugTex, v2f_texcoord);
}
