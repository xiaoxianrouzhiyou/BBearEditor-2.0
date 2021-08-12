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

// Lambert
vec4 getLambertLight()
{
    vec3 light_pos_normalized = normalize(BBLightPosition.xyz);
    vec3 normal_normalized = normalize(BBNormal.xyz);
    vec4 light_color = dot(light_pos_normalized, normal_normalized) * BBLightColor;
    // Ambient
    light_color.xyz += vec3(0.1);
    return light_color;
}

void main()
{
    V_Color = BBColor * getLambertLight();
    gl_Position = BBProjectionMatrix * BBViewMatrix * BBModelMatrix * BBPosition;
}
