varying vec4 V_Texcoord;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;

uniform vec4 lightPosition;
uniform vec4 lightColor;

void main(void)
{
    vec4 pos_data = texture2D(texture0, V_Texcoord.xy);
    vec3 world_pos = (pos_data.xyz * 2.0 - vec3(1.0)) * 1.0;

    vec4 normal_data = texture2D(texture1, V_Texcoord.xy);
    vec3 normal = normal_data.xyz * 2.0 - vec3(1.0);

//    vec4 color_data = texture2D(texture2, V_Texcoord.xy);
//    vec3 color = color_data.rgb;

    float intensity = 0.0;
    if (lightPosition.w == 0.0)
    {
        // directional light
        vec3 object_to_light_source = normalize(lightPosition.xyz);
        intensity = dot(object_to_light_source, normal);
    }
    else
    {
        // other types
    }

    gl_FragColor = vec4(lightColor.xyz * intensity, 1.0);
}
