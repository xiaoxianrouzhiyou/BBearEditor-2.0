attribute vec4 BBPosition;
attribute vec4 BBTexcoord;
attribute vec4 BBNormal;

varying vec3 V_view_dir;
varying vec4 V_texcoord;
varying vec4 V_normal;

uniform mat4 BBProjectionMatrix;
uniform mat4 BBViewMatrix;
uniform mat4 BBModelMatrix;
uniform vec4 BBCameraPosition;

void main()
{
    vec4 world_space_pos = BBModelMatrix * BBPosition;
    V_view_dir = normalize(BBCameraPosition.xyz - world_space_pos.xyz);
    V_texcoord = BBTexcoord;
    V_normal.xyz = mat3(transpose(inverse(BBModelMatrix))) * BBNormal.xyz;
    gl_Position = BBProjectionMatrix * BBViewMatrix * world_space_pos;
}
