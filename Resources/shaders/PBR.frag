varying vec4 V_Texcoord;
varying vec4 V_World_pos;
varying mat3 V_TBN;

uniform vec4 BBColor_Color;
uniform sampler2D Albedo_Map;
uniform sampler2D Normal_Map;
uniform sampler2D Metallic_R_Glossiness_G_AO_B;
uniform float Glossiness;
uniform float Metallic;
uniform float AO;

uniform vec4 BBLightSettings0;
uniform vec4 BBLightPosition;
uniform vec4 BBLightColor;
uniform vec4 BBCameraPosition;

void main(void)
{
    vec3 pbr_albedo = BBColor_Color.rgb * texture2D(Albedo_Map, V_Texcoord.xy).rgb;

    vec3 pbr_normal = vec3(texture2D(Normal_Map, V_Texcoord.xy));
    // 0~1 -> -1~1
    pbr_normal = pbr_normal * 2.0 - vec3(1.0);
    pbr_normal = normalize(pbr_normal);
    pbr_normal = V_TBN * pbr_normal;
    pbr_normal = normalize(pbr_normal);

    // non-metal = 0, metal = 1
    vec4 metallic_tex = texture2D(Metallic_R_Glossiness_G_AO_B, V_Texcoord.xy);
    float pbr_metallic = metallic_tex.r * Metallic;
    float pbr_glossiness = metallic_tex.g * Glossiness;
    float pbr_occlusion = metallic_tex.b * AO;
    float pbr_alpha = 1.0;

    vec3 view_dir = normalize(BBCameraPosition.xyz - V_World_pos.xyz);

    if (BBLightSettings0.x == 1.0)
    {
        vec3 light_dir = normalize(BBLightPosition.xyz);
    }

    gl_FragColor = BBColor_Color * texture2D(Albedo_Map, V_Texcoord.xy);
}
