varying vec4 V_Color;
varying vec4 V_Normal;

uniform vec4 BBLightPosition;
uniform vec4 BBLightColor;

// Lambert
vec4 getLambertLight()
{
    vec3 light_pos_normalized = normalize(BBLightPosition.xyz);
    vec3 normal_normalized = normalize(V_Normal.xyz);
    vec4 light_color = dot(light_pos_normalized, normal_normalized) * BBLightColor;
    // Ambient
    light_color.xyz += vec3(0.1);
    return light_color;
}

void main(void)
{
    gl_FragColor = V_Color * getLambertLight();
}
