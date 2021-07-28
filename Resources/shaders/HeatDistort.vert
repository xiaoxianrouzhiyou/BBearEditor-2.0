attribute vec4 position;
attribute vec4 texcoord;

varying vec4 V_Texcoord;
varying vec4 V_ScreenUV;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

uniform vec4 screenParameters;
uniform vec4 time;

void main()
{
    V_Texcoord.xy = texcoord.xy + vec2(0.0001, 0.0001) * time.z;
    vec4 pos = projectionMatrix * viewMatrix * modelMatrix * position;
    V_ScreenUV.xyz = pos.xyz / pos.w * 0.5 + 0.5; // (-1, 1) ~ (0, 1)
    gl_Position = pos;
}
