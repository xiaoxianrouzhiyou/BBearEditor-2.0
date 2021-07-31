attribute vec4 BBPosition;
attribute vec4 BBNormal;

varying vec4 V_Position;
varying vec4 V_world_pos;
varying vec4 V_Normal;

uniform mat4 BBProjectionMatrix;
uniform mat4 BBViewMatrix;
uniform mat4 BBModelMatrix;

void main()
{
    V_Position = BBPosition;
    // V_Position.y = -V_Position.y;
    V_world_pos = BBModelMatrix * BBPosition;
    V_Normal.xyz = mat3(transpose(inverse(BBModelMatrix))) * BBNormal.xyz;
    gl_Position = BBProjectionMatrix * BBViewMatrix * V_world_pos;
}
