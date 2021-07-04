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
    vec3 light_pos_normalized = normalize(lightPosition.xyz);
    vec3 normal_normalized = normalize(normal.xyz);
    return dot(light_pos_normalized, normal_normalized) * lightColor;
}

void main()
{
    V_Color = color * 0.5 * (getLightColor() + vec4(1.0)) + vec4(0.1);
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * position;
}
