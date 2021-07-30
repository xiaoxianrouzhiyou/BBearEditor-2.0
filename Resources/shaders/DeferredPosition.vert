attribute vec4 BBPosition;

varying vec4 V_WorldPos;

uniform mat4 BBProjectionMatrix;
uniform mat4 BBViewMatrix;
uniform mat4 BBModelMatrix;

void main()
{
    V_WorldPos = BBModelMatrix * BBPosition;
    gl_Position = BBProjectionMatrix * BBViewMatrix * BBModelMatrix * BBPosition;
}
