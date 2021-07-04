varying vec4 V_world_pos;
varying vec4 V_Color;
varying vec4 V_Normal;

uniform vec4 lightSettings0;
uniform vec4 lightSettings1;
uniform vec4 lightPosition;
uniform vec4 lightColor;

void main(void)
{
    vec4 final_color = V_Color;
    vec3 normal = normalize(V_Normal.xyz);
    if (lightPosition.w == 1.0)
    {
        // there is a point light
        float constant_factor = lightSettings1.y;
        float linear_factor = lightSettings1.z;
        float quadric_factor = lightSettings1.w;
        vec3 L = lightPosition.xyz - V_world_pos.xyz;
        float distance = length(L);
        float attenuation = 1.0 / (constant_factor + linear_factor * distance + quadric_factor * quadric_factor * distance);
        L = normalize(L);
        float intensity = max(0.0, dot(L, normal));
        final_color = lightColor * intensity * attenuation;
    }
    gl_FragColor = final_color;
}
