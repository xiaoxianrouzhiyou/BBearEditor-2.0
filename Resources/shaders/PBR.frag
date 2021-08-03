varying vec4 V_Texcoord;
varying vec4 V_World_pos;
varying mat3 V_TBN;

uniform vec4 BBLightPosition;
uniform vec4 BBLightColor;
uniform vec4 BBCameraPosition;

uniform vec4 BBColor_Color;
uniform sampler2D Albedo_Map;
uniform sampler2D Normal_Map;
uniform sampler2D Metallic_R_Roughness_G_AO_B;
uniform float Roughness;
uniform float Metallic;
// uniform float AO;
uniform samplerCube Environment_Cube;

struct PBR
{
    vec3 albedo;
    vec3 normal;
    float metallic;
    float glossiness;
    float roughness;
    float occlusion;
    float alpha;

    vec3 reflect_uvw;
    vec3 view_dir;
};

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

struct GI
{
    Light light;
    IndirectLight indirect_light;
};

vec3 computeSpecular(PBR pbr)
{
    vec3 specular = vec3(0.0);
    // Sample a small cube map to simulate blur
    pbr.roughness = pbr.roughness * (1.7 - 0.7 * pbr.roughness);
    // Sample level
    float mip = pbr.roughness * 6;
    // Sample LOD, to do ...
    specular = textureCube(Environment_Cube, pbr.reflect_uvw).xyz;
    specular *= pbr.occlusion;
    return specular;
}

void standardLightingGI(PBR pbr, inout GI gi)
{
    // diffuse, to do ... Spherical Harmonic Lighting
    gi.indirect_light.diffuse *= pbr.occlusion;
    // specular, cube map
    gi.indirect_light.specular = computeSpecular(pbr);
}

vec3 computeDiffuseAndSpecularFromMetallic(vec3 albedo, float metallic, inout vec3 specular_color, inout float one_minus_reflectivity)
{
    // The non-metallic specular reflection is gray, while the metal has various bright colors
    // depend on metallic
    // gamma
    // vec4 color_space_dielectric_specular = vec4(0.220916301, 0.220916301, 0.220916301, 1.0 - 0.220916301);
    // linear
    // Generally, the reflectivity of non-metallic materials is about 4%
    vec4 color_space_dielectric_specular = vec4(0.04, 0.04, 0.04, 1.0 - 0.04);
    specular_color = mix(color_space_dielectric_specular.xyz, albedo, metallic);
    // the reflectivity of diffuse by the reflectivity of metallic
    // 1 - mix(dielectric_specular, 1, metallic)
    // mix(1 - color_space_dielectric_specular.a, 0, metallic)
    // mix(A, B, v) = A + v * (B - A)
    one_minus_reflectivity = color_space_dielectric_specular.a - metallic * color_space_dielectric_specular.a;
    return albedo * one_minus_reflectivity;
}

// Disney
float computeDisneyDiffuse(float NdotV, float NdotL, float LdotH, float roughness)
{
    // F d90 : fresnel, view from 90d
    float fd90 = 0.5 + 2 * LdotH * LdotH * roughness;
    float light_scatter = 1 + (fd90 - 1) * pow(1 - NdotL, 5);
    float view_scatter = 1 + (fd90 - 1) * pow(1 - NdotV, 5);

    return light_scatter * view_scatter;
}

vec3 safeNormalize(vec3 v)
{
    // Avoid negative
    return v / sqrt(max(0.001, dot(v, v)));
}

float computeSmithJointGGXVisibilityTerm(float NdotL, float NdotV, float roughness)
{
    // approximation
    float a = roughness;
    float lambdaV = NdotL * (NdotV * (1 - a) + a);
    float lambdaL = NdotV * (NdotL * (1 - a) + a);
    return 0.5 / (lambdaV + lambdaL + 0.00001);
}

float computeGGXTerm(float NdotH, float roughness)
{
    float a2 = roughness * roughness;
    float d = (NdotH * a2 - NdotH) * NdotH + 1.0;
    return 1.0 / 3.1415926 * a2 / (d * d + 0.0000001);
}

vec3 computeFresnelTerm(vec3 F0, float cosA)
{
    // F0 = when view_dir and normal overlap, e.g. view water surface from top, small
    // cosA = v, h
    float t = pow(1 - cosA, 5.0);
    return F0 + (1 - F0) * t;
}

vec3 computeFresnelLerp(vec3 F0, vec3 F90, float cosA)
{
    // F0 = when view_dir and normal overlap, e.g. view water surface from top, small
    // F90 = view from the side
    float t = pow(1 - cosA, 5.0);
    return mix(F0, F90, t);
}

vec4 computeBRDF(PBR pbr, GI gi, vec3 specular_color, float one_minus_reflectivity)
{
    // Bidirectional Reflectance Distribution Function
    // Bidirectional: light_dir and view_dir
    // diffuse + gi_diffuse + specular + gi_specular

    float perceptual_roughness = pbr.roughness;
    vec3 half_dir = safeNormalize(gi.light.dir + pbr.view_dir);
    // generally, NdotV should not be negative for visible pixels, but it can happen due to perspective projection and normal mapping
    // can modify with more operation
    // or use abs, but there are a few errors
    float NdotV = abs(dot(pbr.normal, pbr.view_dir));
    float NdotL = clamp(dot(pbr.normal, gi.light.dir), 0.0, 1.0);
    float NdotH = clamp(dot(pbr.normal, half_dir), 0.0, 1.0);

    float LdotV = clamp(dot(gi.light.dir, pbr.view_dir), 0.0, 1.0);
    float LdotH = clamp(dot(gi.light.dir, half_dir), 0.0, 1.0);

    vec3 diffuse = pbr.albedo;
    // Theoretically, we should divide diffuse_term by Pi and not multiply specular_term
    float diffuse_term = computeDisneyDiffuse(NdotV, NdotL, LdotH, perceptual_roughness) * NdotL;
    diffuse = diffuse * (gi.indirect_light.diffuse + gi.light.color * diffuse_term);

    // D : Normal Distribution Function     when half_dir and normal overlap, valid
    // DFG / (4 * cosl * cosv)
    // V = F / (4 * cosl * cosv)
    // * Pi, because we do not divide Pi when computing diffuse
    // better roughness
    float roughness = perceptual_roughness * perceptual_roughness;
    roughness = max(roughness, 0.002);
    float V = computeSmithJointGGXVisibilityTerm(NdotL, NdotV, roughness);
    float D = computeGGXTerm(NdotH, roughness);
    // DG = specular_term
    float specular_term = V * D * 3.1415926;
    // gamma correction
    // specular_term = sqrt(max(0.0001, specular_term));
    specular_term = max(0.0, specular_term * NdotL);
    // handle metallic = 1 and albedo is black
    if (specular_color.r == 0.0 && specular_color.g == 0.0 && specular_color.b == 0.0)
    {
        specular_term *= 0.0;
    }
    else
    {
        specular_term *= 1.0;
    }
    // F = fresnel_term
    float fresnel_term = computeFresnelTerm(specular_color, LdotH);
    vec3 specular = specular_term * gi.light.color * fresnel_term;
    // Image Based Lighting
    // surface_reduction : gi specular is different with reduction
    // fresnel_lerp : The transition of specular at different angles (F0 - F90)
    // reduction = 1 / (roughness ^ 2 + 1)
    // gamma space
    // float surface_reduction = 1.0 - 0.28 * roughness * perceptual_roughness;
    // linear space
    // 0.5 ~ 1
    float surface_reduction = 1.0 / (roughness * roughness + 1.0);
    float grazing_term = clamp(pbr.glossiness + (1 - one_minus_reflectivity), 0.0, 1.0);
    vec3 fresnel_lerp = computeFresnelLerp(specular_color, grazing_term, NdotV);
    vec3 ibl = surface_reduction * gi.indirect_light.specular * fresnel_lerp;

    vec3 c = diffuse + specular + ibl;
    return vec4(c, 1.0);    
}

vec4 standardLighting(PBR pbr, GI gi)
{
    vec4 c = vec4(0.0);
    vec3 specular_color = vec3(0.0);
    float one_minus_reflectivity = 0.0;
    pbr.albedo = computeDiffuseAndSpecularFromMetallic(pbr.albedo, pbr.metallic, specular_color, one_minus_reflectivity);
    // transparent, to do ...

    // BRDF
    c = computeBRDF(pbr, gi, specular_color, one_minus_reflectivity);

    return c;
}

void main(void)
{
    PBR pbr;
    pbr.albedo = BBColor_Color.rgb * texture2D(Albedo_Map, V_Texcoord.xy).rgb;

    pbr.normal = vec3(texture2D(Normal_Map, V_Texcoord.xy));
    // 0~1 -> -1~1
    pbr.normal = pbr.normal * 2.0 - vec3(1.0);
    pbr.normal = normalize(pbr.normal);
    pbr.normal = V_TBN * pbr.normal;
    pbr.normal = normalize(pbr.normal);

    // non-metal = 0, metal = 1
    vec4 metallic_tex = texture2D(Metallic_R_Roughness_G_AO_B, V_Texcoord.xy);
    pbr.metallic = metallic_tex.r * Metallic;
    pbr.roughness = metallic_tex.g * Roughness;
    pbr.glossiness = 1 - pbr.roughness;
    // pbr.occlusion = metallic_tex.b * AO;
    pbr.occlusion = 1.0;
    pbr.alpha = 1.0;

    pbr.view_dir = normalize(BBCameraPosition.xyz - V_World_pos.xyz);
    pbr.reflect_uvw = reflect(-pbr.view_dir, pbr.normal);

    GI gi;
    gi.light.dir = normalize(BBLightPosition.xyz);
    gi.light.color = BBLightColor.xyz;
    standardLightingGI(pbr, gi);

    gl_FragColor = standardLighting(pbr, gi);
}
