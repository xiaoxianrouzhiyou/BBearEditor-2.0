#version 430 core

in vec4 BBPosition;
in vec4 BBNormal;

out vec3 v2f_normal;
out vec3 v2f_world_space_pos;

uniform mat4 BBProjectionMatrix;
uniform mat4 BBViewMatrix;
uniform mat4 BBModelMatrix;

void main()
{
	vec4 world_pos = BBModelMatrix * BBPosition;
	v2f_world_space_pos = world_pos.xyz;
	gl_Position = BBProjectionMatrix * BBViewMatrix * world_pos;
	v2f_normal = normalize(mat3(transpose(inverse(BBViewMatrix * BBModelMatrix))) * BBNormal.xyz);	
}
