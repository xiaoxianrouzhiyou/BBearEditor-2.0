attribute vec4 positionAttr;
attribute vec4 colorAttr;

varying vec4 colorVary;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

void main()
{
    colorVary = colorAttr;
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * positionAttr;
}
