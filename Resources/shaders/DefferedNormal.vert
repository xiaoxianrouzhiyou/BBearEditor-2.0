attribute vec4 positionAttr;
attribute vec4 normalAttr;

varying vec4 V_Normal;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

void main()
{
    V_Normal = modelMatrix * normalAttr;
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * positionAttr;
}
