varying vec4 V_world_pos;
varying vec4 V_Color;
varying vec4 V_Normal;

uniform vec4 lightSettings0;
uniform vec4 lightSettings1;
uniform vec4 lightSettings2;
uniform vec4 lightPosition;
uniform vec4 lightColor;

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
    float intensity = max(0.0, dot(L, normal));
    if (intensity > 0.0)
    {
        vec3 spot_direction = normalize(lightSettings2.xyz);
        // Cosine of the angle between the current point and the spotlight direction
        float current_cos = max(0.0, dot(-L, spot_direction));
        // cutoff cosine
        float cutoff_radian = lightSettings0.z / 2 * 3.14 / 180.0;
        float cutoff_cos = cos(cutoff_radian);
        if (current_cos > cutoff_cos)
        {
            // Within the cutoff range
            float delta = radius - distance;
            intensity = pow(current_cos, lightSettings0.w) * 2.0 * attenuation * delta / radius;
        }
        else
        {
            intensity = 0.0;
        }
    }
    return intensity;
}

void main(void)
{
    vec4 final_color = V_Color;
    vec3 normal = normalize(V_Normal.xyz);
    float radius = lightSettings1.x;
    float constant_factor = lightSettings1.y;
    float linear_factor = lightSettings1.z;
    float quadric_factor = lightSettings1.w;
    vec3 L = lightPosition.xyz - V_world_pos.xyz;
    float distance = length(L);
    float attenuation = 1.0 / (constant_factor + linear_factor * distance + quadric_factor * quadric_factor * distance);
    L = normalize(L);
    if (lightPosition.w == 1.0)
    {
        float intensity;
        if (lightSettings0.x == 2.0)
        {
            // there is a spot light
            intensity = getLambertSpotLightIntensity(normal, radius, distance, attenuation, L);
        }
        else
        {
            // there is a point light
            intensity = getLambertPointLightIntensity(normal, radius, distance, attenuation, L);
        }
        final_color = lightColor * intensity * lightSettings0.y;
    }
    gl_FragColor = final_color;
}
