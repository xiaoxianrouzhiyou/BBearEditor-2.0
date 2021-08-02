varying vec4 V_World_pos;
varying vec4 V_Texcoord;
varying vec4 V_Normal;

uniform sampler2D Albedo_Map;

// main light
uniform vec4 BBLightSettings0;
uniform vec4 BBLightPosition;
uniform vec4 BBLightColor;
uniform vec4 BBCameraPosition;

struct Light
{
    vec3 color;
    vec3 dir;
};

struct IndirectLight
{
    vec3 diffuse;
    vec3 specular;
};

struct GIInput
{
    Light light;
    vec3 world_pos;
    vec3 world_view_dir;
    float attenuation;
    float ambient;
};

struct GI
{
    Light light;
    IndirectLight indirect_light;
};

struct Surface
{
    vec3 albedo;
    vec3 normal;
    float alpha;
};

vec4 lightingLambert(Surface s, GI gi)
{
    vec4 c = vec4(1.0);
    float diff = max(0.0, dot(s.normal, gi.light.dir));
    c.rgb = s.albedo * gi.light.color * diff;
    c.a = s.alpha;

    // add indirect light
    c.rgb += s.albedo * gi.indirect_light.diffuse;

    return c;
}

void main(void)
{
    Surface s;
    s.albedo = texture2D(Albedo_Map, V_Texcoord.xy);
    s.normal = V_Normal;
    s.alpha = 1.0;

    GI gi;
    gi.light.color = BBLightColor.rgb;
    gi.light.dir = normalize(BBLightPosition.xyz);
    gi.indirect_light.diffuse = vec3(0.0);
    gi.indirect_light.specular = vec3(0.0);

    GIInput input;
    input.light = gi.light;
    input.world_pos = V_World_pos.xyz;
    input.world_view_dir = normalize(BBCameraPosition.xyz - V_World_pos.xyz);
    input.attenuation = 1.0;
    input.ambient = 0.0;

    gl_FragColor = lightingLambert(s, gi);
}
