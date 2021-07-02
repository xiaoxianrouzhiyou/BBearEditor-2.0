attribute vec4 position;

varying vec4 V_WorldPos;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

void main()
{
    V_WorldPos = modelMatrix * position;
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * position;
}
