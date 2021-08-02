attribute vec4 BBPosition;
attribute vec4 BBTexcoord;
attribute vec4 BBNormal;

varying vec4 V_World_pos;
varying vec4 V_Texcoord;
varying vec4 V_Normal;

uniform mat4 BBProjectionMatrix;
uniform mat4 BBViewMatrix;
uniform mat4 BBModelMatrix;

void main()
{
    V_World_pos = BBModelMatrix * BBPosition;
    V_Texcoord = BBTexcoord;
    V_Normal.xyz = mat3(transpose(inverse(BBModelMatrix))) * BBNormal.xyz;
    gl_Position = BBProjectionMatrix * BBViewMatrix * V_World_pos;
}
