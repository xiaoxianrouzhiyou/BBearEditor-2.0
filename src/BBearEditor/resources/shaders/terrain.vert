attribute vec4 positionAttr;
attribute vec4 colorAttr;
attribute vec2 texcoordAttr;
attribute vec4 normalAttr;

varying vec4 colorVary;
varying vec2 texcoordVary;
varying vec4 normalVary;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

uniform mat4 IT_modelMatrix;

void main()
{
    colorVary = colorAttr;
    texcoordVary = texcoordAttr;
    normalVary = IT_modelMatrix * normalAttr;
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * positionAttr;
}
