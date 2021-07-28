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

// Lambert
vec4 getLambertLight()
{
    vec3 light_pos_normalized = normalize(lightPosition.xyz);
    vec3 normal_normalized = normalize(normal.xyz);
    vec4 light_color = dot(light_pos_normalized, normal_normalized) * lightColor;
    // Ambient
    light_color.xyz += vec3(0.1);
    return light_color;
}

void main()
{
    V_Color = color * getLambertLight();
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * position;
}
