attribute vec4 BBPosition;
attribute vec4 BBTexcoord;
attribute vec4 BBNormal;

varying vec3 v2f_view_space_pos;
varying vec2 v2f_texcoords;
varying vec3 v2f_normal;

uniform mat4 BBProjectionMatrix;
uniform mat4 BBViewMatrix;
uniform mat4 BBModelMatrix;

void main()
{
    vec4 view_space_pos = BBViewMatrix * BBModelMatrix * BBPosition;
    gl_Position = BBProjectionMatrix * view_space_pos;

    v2f_view_space_pos = view_space_pos.xyz;
    v2f_texcoords = BBTexcoord.xy;
    v2f_normal = mat3(BBModelMatrix) * BBNormal.xyz;
}
