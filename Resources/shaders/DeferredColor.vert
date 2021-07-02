attribute vec4 position;
attribute vec4 texcoord;

varying vec4 V_Texcoord;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

void main()
{
    V_Texcoord = modelMatrix * texcoord;
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * position;
}
