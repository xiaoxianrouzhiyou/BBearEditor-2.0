attribute vec4 BBPosition;
attribute vec4 BBNormal;

varying vec4 V_Normal;

uniform mat4 BBProjectionMatrix;
uniform mat4 BBViewMatrix;
uniform mat4 BBModelMatrix;

void main()
{
    V_Normal = BBModelMatrix * BBNormal;
    gl_Position = BBProjectionMatrix * BBViewMatrix * BBModelMatrix * BBPosition;
}
