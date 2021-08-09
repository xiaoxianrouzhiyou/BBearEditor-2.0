attribute vec4 BBPosition;

varying vec4 V_local_pos;

uniform mat4 BBProjectionMatrix;
uniform mat4 BBViewMatrix;
uniform mat4 BBModelMatrix;

void main()
{
    V_local_pos = BBPosition;
    V_local_pos.y = -V_local_pos.y;
    gl_Position = BBProjectionMatrix * BBViewMatrix * BBModelMatrix * BBPosition;
}
