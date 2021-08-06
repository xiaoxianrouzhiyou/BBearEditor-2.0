attribute vec4 BBPosition;
attribute vec4 BBTexcoord;

varying vec4 V_clip_space_pos;
varying float V_view_space_z;

uniform mat4 BBProjectionMatrix;
uniform mat4 BBViewMatrix;
uniform mat4 BBModelMatrix;

void main()
{
    vec4 view_space_pos = BBViewMatrix * BBModelMatrix * BBPosition;
    V_view_space_z = view_space_pos.z;
    V_clip_space_pos = BBProjectionMatrix * view_space_pos;
    gl_Position = V_clip_space_pos;
}