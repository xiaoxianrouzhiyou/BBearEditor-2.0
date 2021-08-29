#version 330 core

out vec4 FragColor;

in vec2 v2f_texcoords;
in vec3 v2f_world_pos;
in vec3 v2f_normal;

uniform vec4 BBCameraPosition;
uniform vec4 BBLightPosition;
uniform vec4 BBLightColor;
uniform vec4 BBLightSettings0;
uniform vec4 BBLightSettings1;
uniform vec4 BBLightSettings2;

uniform sampler2D Albedo_Map;
uniform sampler2D Normal_Map;
uniform sampler2D Metallic_Map;
uniform sampler2D Roughness_Map;
uniform sampler2D AO_Map;

const float PI = 3.14159265359;


vec3 getNormalFromMap()
{
    vec3 normal = texture(Normal_Map, v2f_texcoords).xyz * 2.0 - 1.0;

    vec3 Q1 = dFdx(v2f_world_pos);
    vec3 Q2 = dFdy(v2f_world_pos);
    vec2 st1 = dFdx(v2f_texcoords);
    vec2 st2 = dFdy(v2f_texcoords);

    vec3 N = normalize(v2f_normal);
    vec3 T = normalize(Q1 * st2.t - Q2 * st1.t);
    vec3 B = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * normal);
}

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

// Fresnel equation returns the percentage of light reflected on the surface of an object,
// that is, the parameter KS in our reflection equation
vec3 calculateFresnelSchlick(float cos_theta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cos_theta, 5.0);
}  

float calculateDistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);

    float nom = a2;
    float denom = (NdotH * NdotH * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    // prevent divide by zero for roughness = 0.0 and NdotH = 1.0
    return nom / max(denom, 0.0000001); 
}

float calculateGeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float nom = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float calculateGeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx1 = calculateGeometrySchlickGGX(NdotV, roughness);
    float ggx2 = calculateGeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}


void main(void)
{
    vec3 albedo = pow(texture(Albedo_Map, v2f_texcoords).rgb, vec3(2.2));
    float metallic = texture(Metallic_Map, v2f_texcoords).r;
    float roughness = texture(Roughness_Map, v2f_texcoords).r;
    float ao = texture(AO_Map, v2f_texcoords).r;

    vec3 N = getNormalFromMap();
    vec3 V = normalize(BBCameraPosition.xyz - v2f_world_pos);

    // Lambert radiance
    vec3 L = vec3(1.0);
    vec3 radiance = vec3(0.0);
    if (BBLightPosition.w == 0.0)
    {
        // there is a directional light
        L = normalize(BBLightPosition.xyz);
        float intensity = max(0.0, dot(N, L));
        radiance = intensity * BBLightColor.rgb;
    }
    else
    {
        float radius = BBLightSettings1.x;
        float constant_factor = BBLightSettings1.y;
        float linear_factor = BBLightSettings1.z;
        float quadric_factor = BBLightSettings1.w;
        L = BBLightPosition.xyz - v2f_world_pos;
        float distance = length(L);
        float attenuation = 1.0 / (constant_factor + linear_factor * distance + quadric_factor * quadric_factor * distance);
        L = normalize(L);

        float intensity;
        if (BBLightSettings0.x == 2.0)
        {
            // there is a spot light
            intensity = getLambertSpotLightIntensity(N, radius, distance, attenuation, L);
        }
        else
        {
            // there is a point light
            intensity = getLambertPointLightIntensity(N, radius, distance, attenuation, L);
        }
        radiance = BBLightColor.rgb * intensity * BBLightSettings0.y;
    }
    vec3 H = normalize(V + L);

    // Cook-Torrance specular BRDF
    // calculate reflectance at normal incidence
    // if dia-electric (like plastic) use F0 of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)
    vec3 F0 = vec3(0.04f); 
    F0 = mix(F0, albedo, metallic);
    vec3 F = calculateFresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);
    float NDF = calculateDistributionGGX(N, H, roughness);   
    float G = calculateGeometrySmith(N, V, L, roughness);      
    
    vec3 numerator = NDF * G * F;
    float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
    // + 0.001 prevent divide by zero
    vec3 specular = numerator / max(denominator, 0.001);
    
    // kS is equal to Fresnel
    vec3 kS = F;
    // for energy conservation, the diffuse and specular light can't be above 1.0 (unless the surface emits light)
    // to preserve, the diffuse component (kD) should equal 1.0 - kS.
    vec3 kD = vec3(1.0) - kS;
    // multiply kD by the inverse metalness such that only non-metals have diffuse lighting,
    // or a linear blend if partly metal (pure metals have no diffuse light).
    kD *= 1.0 - metallic;

    // scale light by NdotL
    float NdotL = max(dot(N, L), 0.0);  

    // outgoing radiance Lo
    // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
    vec3 Lo = (kD * albedo / PI + specular) * radiance * NdotL;

    // ambient lighting
    vec3 ambient = vec3(0.03) * albedo * ao;
    vec3 final_color = ambient + Lo;

    // Lo as a result may grow rapidly (more than 1), but the value is truncated due to the default LDR input.
    // Therefore, before gamma correction, we use tone mapping to map Lo from the value of LDR to the value of HDR.
    // HDR tonemapping
    final_color = final_color / (final_color + vec3(1.0));
    // gamma correct
    final_color = pow(final_color, vec3(1.0 / 2.2));

    FragColor = vec4(final_color, 1.0);
}
