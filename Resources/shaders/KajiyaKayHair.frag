#version 330 core

in vec2 v2f_texcoords;
in vec3 v2f_world_pos;
in vec3 v2f_normal;
in vec3 v2f_view_dir;

out vec4 FragColor;


uniform vec4 BBLightPosition;
uniform vec4 BBLightColor;
uniform vec4 BBLightSettings0;
uniform vec4 BBLightSettings1;
uniform vec4 BBLightSettings2;


uniform sampler2D BaseMap; // Base color (RGB), Cutoff (A)
uniform vec4 BBColor_BaseColor;
uniform vec4 BBColor_ShadowColor;
uniform float Cutoff;

uniform sampler2D ShiftingNoiseMap;

uniform vec4 BBColor_PrimaryColor;
uniform float PrimaryStrength; // 1.5
uniform float PrimaryFade; // 80
uniform float PrimaryShift; // -1~0
uniform float PrimaryDistort; // 0~1

uniform vec4 BBColor_SecondaryColor;
uniform float SecondaryStrength; // 1.5
uniform float SecondaryFade; // 80
uniform float SecondaryShift; // -1~0
uniform float SecondaryDistort; // 0~1


float getLambertPointLightIntensity(vec3 normal, float radius, float distance, float attenuation, vec3 L)
{
    float delta = radius - distance;
    float intensity = 0.0;
    if (delta >= 0)
    {
        intensity = max(0.0, dot(L, normal) * attenuation * delta / radius);
    }
    return intensity;
}

float getLambertSpotLightIntensity(vec3 normal, float radius, float distance, float attenuation, vec3 L)
{
    float intensity = max(0.0, dot(normal, L));
    if (intensity > 0.0)
    {
        vec3 spot_direction = normalize(BBLightSettings2.xyz);
        // Cosine of the angle between the current point and the spotlight direction
        float current_cos = max(0.0, dot(spot_direction, -L));
        // cutoff cosine
        float cutoff_radian = BBLightSettings0.z / 2 * 3.14 / 180.0;
        float cutoff_cos = cos(cutoff_radian);
        if (current_cos > cutoff_cos)
        {
            // Within the cutoff range
            float delta = radius - distance;
            intensity = pow(current_cos, BBLightSettings0.w) * 2.0 * attenuation * delta / radius;
        }
        else
        {
            intensity = 0.0;
        }
    }
    return intensity;
}

vec3 computeAnisotropicSpecular(float noise, vec3 color, float strength, float fade, float shift, float distort, vec3 T, vec3 H, vec3 N, vec3 V)
{
    vec3 specular = vec3(0.0);
    // Shifting specular highlights, T approaches H
    // Overall offset & additional offset given by noise
    // 0~1 -> -0.5~0.5
    float offset = distort * noise - 0.5 + shift * (-1);
    T = normalize(T + offset * H);

    float TdotH = dot(T, H);
    float sinTH = sqrt(1 - TdotH * TdotH);

    // It's dark above and bright below
    // float dirAtten = smoothstep(-1, 0, TdotH);
    // Fresnel can also be used
    float atten = clamp(dot(N, V), 0.0, 1.0);

    sinTH = clamp(sinTH, 0.0, 1.0);
    specular = vec3(atten * (strength * 2) * pow(sinTH, fade * 80));
    specular *= color;

    return specular;
}


void main(void)
{
    vec4 base_color = texture(BaseMap, v2f_texcoords);

    if (base_color.a < Cutoff)
        discard;

    float noise = texture(ShiftingNoiseMap, v2f_texcoords).r;

    vec3 N = normalize(v2f_normal);
    vec3 V = normalize(v2f_view_dir);

    // Lambert diffuse
    vec3 L = normalize(BBLightPosition.xyz);
    float intensity = dot(N, L);
    // brighten, -1~1 -> 0.2~1
    intensity = intensity * 0.4 + 0.6;
    vec3 hair_color = mix(BBColor_ShadowColor.rgb, BBColor_BaseColor.rgb, intensity);
    vec3 diffuse = intensity * hair_color * base_color.rgb * BBLightColor.rgb;

    vec3 H = normalize(L + V);

    vec3 up = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
    vec3 T = normalize(cross(up, N));
	vec3 B = normalize(cross(N, T));

    // Whether to use T or B depends on the UV direction of the hair
    T = B;

    vec3 primary_specular = computeAnisotropicSpecular(noise, BBColor_PrimaryColor.rgb, PrimaryStrength, PrimaryFade, PrimaryShift, PrimaryDistort, T, H, N, V);

    // Double highlights
    vec3 secondary_specular = computeAnisotropicSpecular(noise, BBColor_SecondaryColor.rgb, SecondaryStrength, SecondaryFade, SecondaryShift, SecondaryDistort, T, H, N, V);

    vec3 final_color = diffuse + primary_specular + secondary_specular;

    FragColor = vec4(final_color, 1.0);
}
