#version 330 core

in vec3 v2f_world_pos;
in vec3 v2f_normal;

out vec4 FragColor;

uniform vec4 BBLightPosition;
uniform vec4 BBLightColor;


void main(void)
{
    vec3 final_color = vec3(0.0);

    vec3 N = normalize(v2f_normal);
    vec3 L = normalize(BBLightPosition.xyz);

    float NdotL = max(0.0, dot(N, L));

    // Dichotomous coloring
    final_color = vec3(step(0.3, NdotL));

    FragColor = vec4(final_color, 1.0);
}

