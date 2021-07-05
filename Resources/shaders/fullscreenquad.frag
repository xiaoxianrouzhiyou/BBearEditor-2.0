varying vec4 V_Texcoord;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;

uniform vec4 lightPosition;
uniform vec4 lightColor;
uniform vec4 lightSettings0;
uniform vec4 lightSettings1;

void main(void)
{
    vec4 pos_data = texture2D(texture0, V_Texcoord.xy);
//    vec3 world_pos = (pos_data.xyz * 2.0 - vec3(1.0)) * 1.0;
    vec3 world_pos = pos_data.xyz;

    vec4 normal_data = texture2D(texture1, V_Texcoord.xy);
//    vec3 normal = normal_data.xyz * 2.0 - vec3(1.0);
    vec3 normal = normal_data.xyz;

    vec4 color_data = texture2D(texture2, V_Texcoord.xy);
    vec3 color = color_data.rgb;

    float intensity = 0.0;
    vec3 final_color = vec3(0.0);
    if (lightSettings0.x != 0.0)
    {
        // there is a light
        if (lightPosition.w == 0.0)
        {
            // directional light
            vec3 object_to_light_source = normalize(lightPosition.xyz);
            intensity = dot(object_to_light_source, normal);
            final_color = color * lightColor.xyz * intensity;
        }
        else
        {
            // point light
            float radius = lightSettings1.x;
            float constant_factor = lightSettings1.y;
            float linear_factor = lightSettings1.z;
            float quadric_factor = lightSettings1.w;
            vec3 L = lightPosition.xyz - world_pos.xyz;
            float distance = length(L);
            float attenuation = 1.0 / (constant_factor + linear_factor * distance + quadric_factor * quadric_factor * distance);
            L = normalize(L);
            float intensity = 0.0;
            if (distance <= radius)
            {
                intensity = max(0.0, dot(L, normal));
            }
            final_color = color * lightColor.xyz * intensity * attenuation;
        }
    }
    else
    {
        final_color = color;
    }

    gl_FragColor = vec4(final_color, 1.0);
}
