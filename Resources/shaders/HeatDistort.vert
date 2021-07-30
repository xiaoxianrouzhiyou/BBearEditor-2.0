attribute vec4 BBPosition;
attribute vec4 BBTexcoord;

varying vec4 V_Texcoord;
varying vec4 V_ScreenUV;

uniform mat4 BBProjectionMatrix;
uniform mat4 BBViewMatrix;
uniform mat4 BBModelMatrix;

uniform vec4 BBScreenParameters;
uniform vec4 BBTime;

void main()
{
    V_Texcoord.xy = BBTexcoord.xy + vec2(0.0001, 0.0001) * BBTime.z;
    vec4 pos = BBProjectionMatrix * BBViewMatrix * BBModelMatrix * BBPosition;
    V_ScreenUV.xyz = pos.xyz / pos.w * 0.5 + 0.5; // (-1, 1) ~ (0, 1)
    gl_Position = pos;
}
