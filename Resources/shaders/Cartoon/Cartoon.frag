#version 330 core

in vec3 v2f_world_pos;
in vec2 v2f_texcoords;
in vec3 v2f_normal;

out vec4 FragColor;

uniform vec4 BBCameraPosition;
uniform vec4 BBLightPosition;
uniform vec4 BBLightColor;

uniform sampler2D Albedo;
// specular
uniform float SpecularStrength;
uniform float SpecularFade;
uniform float SpecularSmoothing;
// fresnel
uniform float FresnelStrength;
uniform float FresnelFade;
uniform float FresnelSmoothing;
uniform vec4 BBColor_FresnelColor;

const int Step = 3;


void main(void)
{
    vec3 final_color = vec3(0.0);

    vec3 N = normalize(v2f_normal);
    vec3 L = normalize(BBLightPosition.xyz);
    vec3 V = normalize(BBCameraPosition.xyz - v2f_world_pos);
    vec3 H = normalize(L + V);

    float NdotL = max(0.0, dot(N, L));
    float NdotH = max(0.0, dot(N, H));
    float NdotV = max(0.0, dot(N, V));

    // Dichotomous coloring and multiple level coloring
    float level = ceil(NdotL * Step) / Step;
    final_color += vec3(level);

    // specular
    float specular = SpecularStrength * 5 * pow(NdotH, SpecularFade * 80);
    specular = smoothstep(0.5, 0.5 + SpecularSmoothing, specular);
    final_color += BBLightColor.rgb * specular;

    // fresnel
    float fresnel = 1 - NdotV;
    fresnel = FresnelStrength * 5 * pow(fresnel, FresnelFade);
    fresnel = smoothstep(0.5, 0.5 + FresnelSmoothing, fresnel);
    final_color += BBColor_FresnelColor.rgb * fresnel;

    vec4 base_color = texture(Albedo, v2f_texcoords);
    final_color += base_color.rgb;

    FragColor = vec4(final_color, 1.0);
}

