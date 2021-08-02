varying mat3 V_TBN;
varying vec4 V_Texcoord;

uniform sampler2D Normal_Map;
uniform sampler2D Base_Map;
uniform vec4 BBLightSettings0;
uniform vec4 BBLightPosition;

void main(void)
{
    vec3 normal = vec3(texture2D(Normal_Map, V_Texcoord.xy));
    // 0~1 -> -1~1
    normal = normal * 2.0 - vec3(1.0);
    normal = normalize(normal);
    normal = V_TBN * normal;
    normal = normalize(normal);

    float intensity = 0.0;
    if (BBLightSettings0.x == 1.0)
    {
        // Lambert
        vec3 L = normalize(BBLightPosition.xyz);
        intensity = max(0.0, dot(normal, L));
    }

    vec4 final_color = texture2D(Base_Map, V_Texcoord.xy);
    final_color.xyz *= intensity;
    gl_FragColor = final_color;
}
