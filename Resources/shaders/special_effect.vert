attribute vec4 position;
attribute vec4 texcoord;
attribute vec4 color;

varying vec4 V_Color;
varying vec4 V_Texcoord;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

void main()
{
    V_Color = color;
    V_Texcoord = texcoord;

    gl_Position = projectionMatrix * viewMatrix * modelMatrix * position;
}
