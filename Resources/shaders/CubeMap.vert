attribute vec4 BBPosition;

varying vec4 V_Position;

uniform mat4 BBProjectionMatrix;
uniform mat4 BBViewMatrix;
uniform mat4 BBModelMatrix;

void main()
{
    V_Position = BBPosition;
    V_Position.y = -V_Position.y;
    gl_Position = BBProjectionMatrix * BBViewMatrix * BBModelMatrix * BBPosition;
}
