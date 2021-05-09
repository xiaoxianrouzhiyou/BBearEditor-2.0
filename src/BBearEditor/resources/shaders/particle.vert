attribute vec4 positionAttr;
attribute vec4 colorAttr;
attribute vec4 normalAttr;

varying vec4 colorVary;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

void main()
{
    colorVary = colorAttr;
    //粒子大小
    gl_PointSize = 64.0;
    //位置偏移
    vec4 pos = vec4(positionAttr.x + normalAttr.x, positionAttr.y + normalAttr.y, positionAttr.z + normalAttr.z, 1.0);
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * pos;
}
