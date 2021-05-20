attribute vec4 positionAttr;
attribute vec4 colorAttr;
attribute vec4 normalAttr;

varying vec4 colorVary;
varying vec4 normalVary;
varying vec4 worldPositionVary;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat4 IT_modelMatrix;

void main()
{
    colorVary = colorAttr;
    normalVary = IT_modelMatrix * normalAttr;
    worldPositionVary = modelMatrix * positionAttr;
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * positionAttr;
}
