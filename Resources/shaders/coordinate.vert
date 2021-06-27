attribute vec4 position;
attribute vec4 color;

varying vec4 V_Color;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

void main()
{
    V_Color = color;
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * position;
}
