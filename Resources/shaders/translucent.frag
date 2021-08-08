varying vec3 V_view_dir;
varying vec4 V_texcoord;
varying vec4 V_normal;

uniform vec4 BBLightPosition;
uniform vec4 BBLightColor;


uniform float Normal_Distortion;
uniform float Attenuation;
uniform float Strength;
uniform sampler2D Thickness_Map;
uniform float Thickness;


vec3 lightingTranslucent(vec3 light_dir, vec3 normal, vec3 light_color, float thickness)
{
    vec3 H = light_dir + normal * Normal_Distortion;
    float _LdotV = dot(-H, V_view_dir);
    float I = pow(clamp(_LdotV, 0.0, 1.0), Attenuation) * Strength;
    return I * thickness * light_color;
}

void main(void)
{
    Attenuation *= 10.0;
    vec4 final_color = vec4(1.0);

    // diffuse
    vec3 N = normalize(V_normal);
    vec3 L = normalize(BBLightPosition.xyz);
    float NdotL = max(0.3, dot(N, L));
    final_color *= NdotL;

    float thickness_tex = 1 - texture2D(Thickness_Map, V_texcoord.xy).r;
    thickness_tex = mix(1, thickness_tex, Thickness);

    // transmission
    // Facing the light, the effect is strong
    
    final_color.xyz += lightingTranslucent(L, N, BBLightColor, thickness_tex);

    gl_FragColor = final_color;
}
