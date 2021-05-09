attribute vec4 positionAttr;
attribute vec2 texcoordAttr;

varying vec2 texcoordVary;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

void main()
{
    texcoordVary = texcoordAttr;
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * positionAttr;
}
