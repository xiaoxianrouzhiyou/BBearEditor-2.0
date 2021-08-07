attribute vec4 BBPosition;
attribute vec4 BBTexcoord;
attribute vec4 BBNormal;
attribute vec4 BBTangent;
attribute vec4 BBBiTangent;

varying vec4 V_texcoord;
varying vec4 V_normal_uv;
varying vec4 V_world_space_pos;
varying vec4 V_world_space_normal;
varying vec4 V_clip_space_pos;
varying float V_view_space_z;
varying mat3 V_TBN;

uniform mat4 BBProjectionMatrix;
uniform mat4 BBViewMatrix;
uniform mat4 BBModelMatrix;
uniform vec4 BBTime;

uniform float Speed;
uniform vec4 BBTO_Foam;
uniform vec4 BBTO_Normal_Map;

void main()
{
    V_world_space_pos = BBModelMatrix * BBPosition;
    V_world_space_normal.xyz = mat3(transpose(inverse(BBModelMatrix))) * BBNormal.xyz;

    // flow
    vec2 offset = vec2(0.0005) * Speed * BBTime.z;
    // Foam uv do not use BBTexcoord, otherwise, the effect will be affected with scale
    V_texcoord.xy = V_world_space_pos.xz * BBTO_Foam.xy + offset;
    // distort uv
    vec2 normal_uv = BBTexcoord * BBTO_Normal_Map.xy;
    V_normal_uv.xy = normal_uv + offset;
    V_normal_uv.zw = normal_uv + offset * vec2(-1.07, 1.35);

    vec4 view_space_pos = BBViewMatrix * V_world_space_pos;
    V_view_space_z = view_space_pos.z;
    V_clip_space_pos = BBProjectionMatrix * view_space_pos;

    vec3 T = normalize(vec3(BBModelMatrix * BBTangent));
    vec3 B = normalize(vec3(BBModelMatrix * BBBiTangent));
    vec3 N = normalize(vec3(BBModelMatrix * BBNormal));
    V_TBN = mat3(T, B, N);

    gl_Position = V_clip_space_pos;
}