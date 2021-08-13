#version 430 core

in vec4 BBPosition;
in vec4 BBTexcoord;
in vec4 BBNormal;

out vec2 v2f_texcoord;
out vec3 v2f_normal;
out vec3 v2f_view_space_pos;

uniform mat4 BBProjectionMatrix;
uniform mat4 BBViewMatrix;
uniform mat4 BBModelMatrix;

void main()
{
    vec4 view_space_pos = BBViewMatrix * BBModelMatrix * BBPosition;
	gl_Position = BBProjectionMatrix * view_space_pos;
	v2f_texcoord = BBTexcoord.xy;
	v2f_normal = normalize(mat3(transpose(inverse(BBViewMatrix * BBModelMatrix))) * BBNormal.xyz);	
	v2f_view_space_pos = view_space_pos.xyz;
}
