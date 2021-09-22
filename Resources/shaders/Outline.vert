attribute vec4 BBPosition;
attribute vec4 BBTexcoord;
attribute vec4 BBNormal;

varying vec4 V_Texcoord;

uniform mat4 BBProjectionMatrix;
uniform mat4 BBViewMatrix;
uniform mat4 BBModelMatrix;

uniform float OutlineWidth;

void main()
{
    V_Texcoord = BBTexcoord;
    BBPosition += BBNormal * OutlineWidth * 0.05;
    gl_Position = BBProjectionMatrix * BBViewMatrix * BBModelMatrix * BBPosition;
}
