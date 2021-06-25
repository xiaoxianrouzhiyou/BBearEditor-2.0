attribute vec4 positionAttr;

varying vec4 V_WorldPos;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

void main()
{
    V_WorldPos = modelMatrix * positionAttr;
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * positionAttr;
}
