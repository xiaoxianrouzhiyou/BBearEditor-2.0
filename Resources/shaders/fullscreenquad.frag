varying vec4 V_Texcoord;

uniform sampler2D BBTexture0;
uniform sampler2D BBTexture1;
uniform sampler2D BBTexture2;

uniform vec4 BBLightPosition;
uniform vec4 BBLightColor;
uniform vec4 BBLightSettings0;
uniform vec4 BBLightSettings1;

void main(void)
{
    vec4 pos_data = texture2D(BBTexture0, V_Texcoord.xy);
//    vec3 world_pos = (pos_data.xyz * 2.0 - vec3(1.0)) * 1.0;
    vec3 world_pos = pos_data.xyz;

    vec4 normal_data = texture2D(BBTexture1, V_Texcoord.xy);
//    vec3 normal = normal_data.xyz * 2.0 - vec3(1.0);
    vec3 normal = normal_data.xyz;

    vec4 color_data = texture2D(BBTexture2, V_Texcoord.xy);
    vec3 color = color_data.rgb;

    float intensity = 0.0;
    vec3 final_color = vec3(0.0);
    if (BBLightSettings0.x != 0.0)
    {
        // there is a light
        if (BBLightPosition.w == 0.0)
        {
            // directional light
            vec3 object_to_light_source = normalize(BBLightPosition.xyz);
            intensity = dot(object_to_light_source, normal);
            final_color = color * BBLightColor.xyz * intensity;
        }
        else
        {
            // point light
            float radius = BBLightSettings1.x;
            float constant_factor = BBLightSettings1.y;
            float linear_factor = BBLightSettings1.z;
            float quadric_factor = BBLightSettings1.w;
            vec3 L = BBLightPosition.xyz - world_pos.xyz;
            float distance = length(L);
            float attenuation = 1.0 / (constant_factor + linear_factor * distance + quadric_factor * quadric_factor * distance);
            L = normalize(L);
            float delta = radius - distance;
            float intensity = 0.0;
            if (delta >= 0)
            {
                intensity = max(0.0, dot(L, normal) * attenuation * delta / radius);
            }
            final_color = color * BBLightColor.xyz * intensity;
        }
    }
    else
    {
        final_color = color;
    }
    gl_FragColor = vec4(final_color, 1.0);
}
