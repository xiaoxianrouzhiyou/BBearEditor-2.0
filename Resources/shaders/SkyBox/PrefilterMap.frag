#version 330 core
#extension GL_NV_shadow_samplers_cube : enable

in vec3 v2f_local_pos;

layout (location = 0) out vec4 FragColor;

uniform samplerCube EnvironmentMap;
uniform float Roughness;

const float PI = 3.14159265359;
const uint SAMPLE_COUNT = 1024u;
// resolution of source cubemap (per face)
const float Resolution = 512.0; 


float computeDistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float nom = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

// http://holger.dammertz.org/stuff/notes_HammersleyOnHemisphere.html
// efficient VanDerCorpus calculation.
float computeRadicalInverse_VdC(uint bits) 
{
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}

vec2 computeHammersley(uint i, uint N)
{
	return vec2(float(i) / float(N), computeRadicalInverse_VdC(i));
}

vec3 computeImportanceSampleGGX(vec2 Xi, vec3 N, float roughness)
{
	float a = roughness * roughness;
	
	float phi = 2.0 * PI * Xi.x;
	float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a * a - 1.0) * Xi.y));
	float sinTheta = sqrt(1.0 - cosTheta * cosTheta);
	
	// from spherical coordinates to cartesian coordinates - halfway vector
    // r = 1
	vec3 H;
	H.x = cos(phi) * sinTheta;
	H.y = sin(phi) * sinTheta;
	H.z = cosTheta;
	
	// from tangent-space H vector to world-space sample vector
	vec3 up = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
	vec3 tangent = normalize(cross(up, N));
	vec3 bitangent = cross(N, tangent);
	
	vec3 sample_vec = tangent * H.x + bitangent * H.y + N * H.z;
	return normalize(sample_vec);
}


void main(void)
{
    vec3 N = normalize(v2f_local_pos);
    // make the simplyfying assumption that V equals R equals the normal 
    vec3 R = N;
    vec3 V = R;

    vec3 prefiltered_color = vec3(0.0);
    float total_weight = 0.0;

    // Monte Carlo
    for (uint i = 0u; i < SAMPLE_COUNT; i++)
    {
        // generates a sample vector that's biased towards the preferred alignment direction (importance sampling).
        vec2 Xi = computeHammersley(i, SAMPLE_COUNT);
        vec3 H = computeImportanceSampleGGX(Xi, N, Roughness);
        vec3 L = normalize(2.0 * dot(V, H) * H - V);

        float NdotL = max(dot(N, L), 0.0);
        if (NdotL > 0.0)
        {
            // it may still be insufficient at some coarser MIP levels, resulting in dot patterns around bright areas
            // We do not sample the environment map directly
            // sample from the environment's mip level based on roughness/pdf
            float D = computeDistributionGGX(N, H, Roughness);
            float NdotH = max(dot(N, H), 0.0);
            float HdotV = max(dot(H, V), 0.0);
            float pdf = D * NdotH / (4.0 * HdotV) + 0.0001; 
            
            float saTexel = 4.0 * PI / (6.0 * Resolution * Resolution);
            float saSample = 1.0 / (float(SAMPLE_COUNT) * pdf + 0.0001);

            float mip_level = Roughness == 0.0 ? 0.0 : 0.5 * log2(saSample / saTexel); 
            
            prefiltered_color += textureLod(EnvironmentMap, L, mip_level).rgb * NdotL;
            total_weight += NdotL;
        }
    }

    prefiltered_color = prefiltered_color / total_weight;

    FragColor = vec4(prefiltered_color, 1.0);
}

