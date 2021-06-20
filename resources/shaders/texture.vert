attribute vec4 positionAttr;
attribute vec4 texcoordAttr;

varying vec4 texcoordVary;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

void main()
{
    texcoordVary = texcoordAttr;
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * positionAttr;
}
