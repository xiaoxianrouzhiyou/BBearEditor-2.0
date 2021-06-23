attribute vec4 positionAttr;
attribute vec4 colorAttr;
attribute vec4 normalAttr;

varying vec4 colorVary;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform vec4 lightPositionUniform;
uniform vec4 lightColorUniform;

vec4 getLightColor()
{
    return dot(lightPositionUniform.xyz, normalAttr.xyz) * lightColorUniform;
}

void main()
{
    colorVary = colorAttr * getLightColor();
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * positionAttr;
}
