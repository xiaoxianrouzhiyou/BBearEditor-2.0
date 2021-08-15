#version 430 core

out vec2 v2f_texcoord;
out vec3 v2f_view_space_pos;

layout (location = 0) out vec4 Albedo_Metallic;

uniform sampler2D DiffuseTex;

void main(void)
{
    float gamma = 2.2;
	vec3 diffuse_color = pow(texture(DiffuseTex, v2f_texcoord).rgb, vec3(gamma));	
	vec3 mapped = vec3(1.0) - exp(-diffuse_color);
	mapped = pow(mapped, vec3(1.0 / gamma));
	Albedo_Metallic = vec4(mapped, 1.0);
	float alpha = textureLod(DiffuseTex, v2f_texcoord, 0).a;
	if (alpha != 1.0)
        discard;
}
