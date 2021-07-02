attribute vec4 position;
attribute vec4 normal;

varying vec4 V_Normal;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

void main()
{
    V_Normal = modelMatrix * normal;
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * position;
}
