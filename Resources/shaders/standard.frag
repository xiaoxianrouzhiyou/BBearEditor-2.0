varying vec4 V_world_pos;
varying vec4 V_Color;
varying vec4 V_Normal;

uniform vec4 lightSettings0;
uniform vec4 lightSettings1;
uniform vec4 lightPosition;
uniform vec4 lightColor;

vec4 getLambertPointLightColor(vec3 normal)
{
    float radius = lightSettings1.x;
    float constant_factor = lightSettings1.y;
    float linear_factor = lightSettings1.z;
    float quadric_factor = lightSettings1.w;
    vec3 L = lightPosition.xyz - V_world_pos.xyz;
    float distance = length(L);
    float attenuation = 1.0 / (constant_factor + linear_factor * distance + quadric_factor * quadric_factor * distance);
    L = normalize(L);
    float delta = radius - distance;
    float intensity = 0.0;
    if (delta >= 0)
    {
        intensity = max(0.0, dot(L, normal) * attenuation * delta / radius);
    }
    return lightColor * intensity;
}

void main(void)
{
    vec4 final_color = V_Color;
    vec3 normal = normalize(V_Normal.xyz);
    if (lightPosition.w == 1.0)
    {
        // there is a point light
        
        final_color = getLambertPointLightColor(normal);
    }
    gl_FragColor = final_color;
}
