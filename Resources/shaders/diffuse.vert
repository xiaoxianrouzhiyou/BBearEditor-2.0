attribute vec4 BBPosition;
attribute vec4 BBColor;
attribute vec4 BBNormal;

varying vec4 V_Color;
varying vec4 V_Normal;

uniform mat4 BBProjectionMatrix;
uniform mat4 BBViewMatrix;
uniform mat4 BBModelMatrix;

void main()
{
    V_Color = BBColor;
    V_Normal.xyz = mat3(transpose(inverse(BBModelMatrix))) * BBNormal.xyz;
    gl_Position = BBProjectionMatrix * BBViewMatrix * BBModelMatrix * BBPosition;
}
