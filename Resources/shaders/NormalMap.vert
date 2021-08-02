attribute vec4 BBPosition;
attribute vec4 BBTexcoord;
attribute vec4 BBNormal;
attribute vec4 BBTangent;
attribute vec4 BBBiTangent;

uniform mat4 BBProjectionMatrix;
uniform mat4 BBViewMatrix;
uniform mat4 BBModelMatrix;

varying mat3 V_TBN;
varying vec4 V_Texcoord;

void main()
{
    vec3 T = normalize(vec3(BBModelMatrix * BBTangent));
    vec3 B = normalize(vec3(BBModelMatrix * BBBiTangent));
    vec3 N = normalize(vec3(BBModelMatrix * BBTangent));
    V_TBN = mat3(T, B, N);
    V_Texcoord = BBTexcoord;
    gl_Position = BBProjectionMatrix * BBViewMatrix * BBModelMatrix * BBPosition;
}
