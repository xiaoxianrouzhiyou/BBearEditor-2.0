attribute vec4 position;
attribute vec4 color;
attribute vec4 texcoord;
attribute vec4 normal;

varying vec4 V_Color;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform vec4 lightPosition;
uniform vec4 lightColor;

vec4 getLightColor()
{
    return dot(lightPosition.xyz, normal.xyz) * lightColor;
}

void main()
{
    V_Color = color * getLightColor();
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * position;
}
