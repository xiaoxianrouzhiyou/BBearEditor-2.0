attribute vec4 positionAttr;
attribute vec4 colorAttr;
attribute vec2 texcoordAttr;
attribute vec4 normalAttr;

varying vec4 colorVary;
varying vec2 texcoordVary;
varying vec4 normalVary;
varying vec4 worldPositionVary;
//视口坐标
varying vec4 viewSpacePositionVary;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
//正确计算法线
uniform mat4 IT_modelMatrix;

//阴影
uniform mat4 lightViewUniform;
uniform mat4 lightProjectionUniform;
varying vec4 lightSpacePosVary;

void main()
{
    colorVary = colorAttr;
    texcoordVary = texcoordAttr;
    normalVary = IT_modelMatrix * normalAttr;
    worldPositionVary = modelMatrix * positionAttr;
    viewSpacePositionVary = viewMatrix * worldPositionVary;
    lightSpacePosVary = lightProjectionUniform * lightViewUniform * worldPositionVary;
    gl_Position = projectionMatrix * viewSpacePositionVary;
}
