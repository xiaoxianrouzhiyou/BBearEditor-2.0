attribute vec4 BBPosition;
attribute vec4 BBTexcoord;
attribute vec4 BBNormal;
attribute vec4 BBTangent;
attribute vec4 BBBiTangent;

varying vec4 V_Texcoord;
varying vec4 V_World_pos;
varying mat3 V_TBN;

uniform mat4 BBProjectionMatrix;
uniform mat4 BBViewMatrix;
uniform mat4 BBModelMatrix;

void main()
{
    V_Texcoord = BBTexcoord;
    V_World_pos = BBModelMatrix * BBPosition;
    vec3 T = normalize(vec3(BBModelMatrix * BBTangent));
    vec3 B = normalize(vec3(BBModelMatrix * BBBiTangent));
    vec3 N = normalize(vec3(BBModelMatrix * BBTangent));
    V_TBN = mat3(T, B, N);
    gl_Position = BBProjectionMatrix * BBViewMatrix * V_World_pos;
}
