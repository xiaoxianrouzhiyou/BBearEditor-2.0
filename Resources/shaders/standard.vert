attribute vec4 position;
attribute vec4 color;
attribute vec4 texcoord;
attribute vec4 normal;

varying vec4 V_world_pos;
varying vec4 V_Color;
varying vec4 V_Normal;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform vec4 lightPosition;
uniform vec4 lightColor;

void main()
{
    V_world_pos = modelMatrix * position;
    V_Color = color;
    V_Normal = normal;
    gl_Position = projectionMatrix * viewMatrix * V_world_pos;
}
