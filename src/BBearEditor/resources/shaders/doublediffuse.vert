attribute vec4 positionAttr;
attribute vec4 colorAttr;
attribute vec2 texcoordAttr;
attribute vec4 normalAttr;

varying vec4 colorVary;
varying vec2 texcoordVary;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

void main()
{
    colorVary = colorAttr;
    texcoordVary = texcoordAttr;
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * positionAttr;
}
