attribute vec4 BBPosition;
attribute vec4 BBColor;
attribute vec4 BBNormal;

varying vec4 V_color;

uniform mat4 BBProjectionMatrix;
uniform mat4 BBViewMatrix;
uniform mat4 BBModelMatrix;

void main()
{
    V_color = BBColor;
    gl_PointSize = 64.0;
    vec4 pos = vec4(BBPosition.xyz + BBNormal.xyz, 1.0);
    gl_Position = BBProjectionMatrix * BBViewMatrix * BBModelMatrix * pos;
}
