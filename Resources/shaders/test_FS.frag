#version 430 core

in G2F
{
	vec4 color;
    vec3 normal;
} g2f;

out vec4 FragColor;

uniform vec4 BBLightPosition;
uniform vec4 BBLightColor;

// Lambert
vec4 getLambertLight()
{
    vec3 light_pos_normalized = normalize(BBLightPosition.xyz);
    vec3 normal_normalized = normalize(g2f.normal);
    vec4 light_color = dot(light_pos_normalized, normal_normalized) * BBLightColor;
    return light_color;
}

void main(void)
{
    FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
