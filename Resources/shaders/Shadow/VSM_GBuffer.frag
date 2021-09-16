#version 430 core

in vec3 v2f_normal;
in vec3 v2f_world_space_pos;

layout (location = 0) out vec4 AlbedoAndMetallic;
layout (location = 1) out vec4 NormalAndDoubleRoughness;
layout (location = 2) out vec4 Position;


void main(void)
{
    vec3 albedo = vec3(0.0f, 1.0f, 0.0f); // diffuse color
	AlbedoAndMetallic = vec4(albedo, 0.0f);
	vec3 normal = normalize(v2f_normal);
	float roughness = 0.2f;
	NormalAndDoubleRoughness = vec4(normal, roughness * roughness);
	Position = vec4(v2f_world_space_pos, 1.0f);
}
