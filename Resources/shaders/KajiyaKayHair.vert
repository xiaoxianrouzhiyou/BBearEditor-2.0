attribute vec4 BBPosition;
attribute vec4 BBTexcoord;
attribute vec4 BBTangent;
attribute vec4 BBBiTangent;

varying vec4 V_texcoord;
varying vec4 V_world_space_tangent;
varying vec4 V_world_space_bitangent;
varying vec4 V_view_dir;

uniform mat4 BBProjectionMatrix;
uniform mat4 BBViewMatrix;
uniform mat4 BBModelMatrix;
uniform vec4 BBCameraPosition;

void main()
{
    V_texcoord = BBTexcoord;
    V_world_space_tangent = BBModelMatrix * BBTangent;
    V_world_space_bitangent = BBModelMatrix * BBBiTangent;
    vec4 world_space_pos = BBModelMatrix * BBPosition;
    V_view_dir.xyz = BBCameraPosition.xyz - world_space_pos.xyz;
    V_view_dir.a = 1.0;
    gl_Position = BBProjectionMatrix * BBViewMatrix * world_space_pos;
}
