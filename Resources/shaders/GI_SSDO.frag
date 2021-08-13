#version 430 core

in vec2 v2f_texcoord;

layout (location = 0) out float SSAOTex;
layout (location = 1) out vec4 SSDOTex;

uniform vec4 BBCameraParameters;
uniform mat4 BBProjectionMatrix;
uniform sampler2D LightTex;
uniform sampler2D NormalTex;
uniform sampler2D PositionTex;
uniform sampler2D NoiseTex;
uniform vec4 Samples[64];

const int kernel_size = 64;
const float radius = 1.0; 

float linearizeDepth(float depth)
{
    float z_near = BBCameraParameters.z;
    float z_far = BBCameraParameters.w;
    float z = depth * 2.0 - 1.0; 
    return (2.0 * z_near * z_far) / (z_far + z_near - z * (z_far - z_near));    
}

void main(void)
{
    float width = BBCameraParameters.x;
    float height = BBCameraParameters.y;

    vec3 pos = texture(PositionTex, v2f_texcoord).xyz;
    
    vec2 noise_scale = vec2(width, height) / 4.0;
    vec3 N = texture(NormalTex, v2f_texcoord).xyz;
    vec3 random_vec = texture(NoiseTex, noise_scale * v2f_texcoord).xyz;
    vec3 T = normalize(random_vec - N * dot(random_vec, N));
    vec3 B = cross(N, T);
    mat3 TBN = mat3(T, B, N);

    float occlusion = 0.0;
    vec3 indirect_light = vec3(0.0);

    for (int i = 0; i < kernel_size; i++)
    {
        vec3 sample_point = TBN * Samples[i].xyz;
        sample_point = pos + sample_point * radius;
        vec4 offset = vec4(sample_point, 1.0);
        offset = BBProjectionMatrix * offset;
        offset.xyz /= offset.w;
        offset.xyz = offset.xyz * 0.5 + 0.5;

        if (offset.x > 1.0 || offset.x < 0.0 || offset.y > 1.0 || offset.y < 0.0)
        {
            continue;
        }

        vec4 position_tex = texture(PositionTex, offset.xy);
        float sample_depth = -position_tex.w;

        float tag = sample_depth >= sample_point.z ? 1.0 : 0.0;

        float weight = smoothstep(0.0, 1.0, radius / abs(pos.z - sample_depth));
        occlusion += tag * weight;

        vec3 sample_normal = texture(NormalTex, offset.xy).xyz;
        vec3 sample_pos = position_tex.xyz;
        vec3 sample_color = texture(LightTex, offset.xy).xyz;
        indirect_light += (1 - tag) * max(dot(N, normalize(pos - sample_pos)), 0.0) * sample_color;
    }
    occlusion = 1.0 - (occlusion / kernel_size);
    indirect_light /= kernel_size;

    SSAOTex = occlusion;
    SSDOTex = vec4(indirect_light * 5.0, 1.0);
}
