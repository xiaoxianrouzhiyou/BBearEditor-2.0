attribute vec4 BBPosition;
attribute vec4 BBColor;
attribute vec4 BBTexcoord;
attribute vec4 BBNormal;

varying vec4 V_world_pos;
varying vec4 V_Color;
varying vec4 V_Normal;
varying vec4 V_world_pos_light_space;

uniform mat4 BBProjectionMatrix;
uniform mat4 BBViewMatrix;
uniform mat4 BBModelMatrix;

uniform mat4 BBLightProjectionMatrix;
uniform mat4 BBLightViewMatrix;

void main()
{
    V_world_pos = BBModelMatrix * BBPosition;
    V_Color = BBColor;
    V_Normal.xyz = mat3(transpose(inverse(BBModelMatrix))) * BBNormal.xyz;
    V_Normal.a = 1.0;
    V_Normal = normalize(V_Normal);
    V_world_pos_light_space = BBLightProjectionMatrix * BBLightViewMatrix * V_world_pos;
    gl_Position = BBProjectionMatrix * BBViewMatrix * V_world_pos;
}
