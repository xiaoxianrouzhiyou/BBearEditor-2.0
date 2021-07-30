attribute vec4 BBPosition;
attribute vec4 BBTexcoord;
attribute vec4 BBColor;

varying vec4 V_Color;
varying vec4 V_Texcoord;

uniform mat4 BBProjectionMatrix;
uniform mat4 BBViewMatrix;
uniform mat4 BBModelMatrix;

void main()
{
    V_Color = BBColor;
    V_Texcoord = BBTexcoord;

    gl_Position = BBProjectionMatrix * BBViewMatrix * BBModelMatrix * BBPosition;
}
