attribute vec4 BBPosition;
attribute vec4 BBColor;
attribute vec4 BBTexcoord;
attribute vec4 BBNormal;

varying vec4 V_Color;

uniform mat4 BBProjectionMatrix;
uniform mat4 BBViewMatrix;
uniform mat4 BBModelMatrix;
uniform vec4 BBLightPosition;
uniform vec4 BBLightColor;

vec4 getLightColor()
{
    vec3 light_pos_normalized = normalize(BBLightPosition.xyz);
    vec3 normal_normalized = normalize(BBNormal.xyz);
    return dot(light_pos_normalized, normal_normalized) * BBLightColor;
}

void main()
{
    V_Color = BBColor * 0.5 * (getLightColor() + vec4(1.0)) + vec4(0.1);
    gl_Position = BBProjectionMatrix * BBViewMatrix * BBModelMatrix * BBPosition;
}
