attribute vec4 BBPosition;
attribute vec4 BBTexcoord;

varying vec2 V_texcoord;
varying vec4 V_clip_space_pos;
varying float V_view_space_z;

uniform mat4 BBProjectionMatrix;
uniform mat4 BBViewMatrix;
uniform mat4 BBModelMatrix;
uniform vec4 BBTime;

uniform float Speed;
uniform vec4 BBTO_Foam;

void main()
{
    vec4 world_space_pos = BBModelMatrix * BBPosition;

    // flow
    // do not use BBTexcoord, otherwise, the effect will be affected with scale
    V_texcoord = world_space_pos.xz * BBTO_Foam.xy  + vec2(0.0005) * Speed * BBTime.z;

    vec4 view_space_pos = BBViewMatrix * world_space_pos;
    V_view_space_z = view_space_pos.z;
    V_clip_space_pos = BBProjectionMatrix * view_space_pos;
    gl_Position = V_clip_space_pos;
}