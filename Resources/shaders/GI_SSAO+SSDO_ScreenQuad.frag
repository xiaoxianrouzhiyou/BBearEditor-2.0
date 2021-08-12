#version 430 core

in vec2 v2f_texcoord;

layout (location = 0) out vec4 FragColor;

uniform vec4 BBCameraParameters;
uniform mat4 BBProjectionMatrix;
uniform sampler2D AlbedoTex;
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
    vec2 noise_scale = vec2(width, height) / 4.0;
    vec3 pos = texture(PositionTex, v2f_texcoord).xyz;
    vec3 N = texture(NormalTex, v2f_texcoord).xyz;
    vec3 random_vec = texture(NoiseTex, noise_scale * v2f_texcoord).xyz;
    vec3 T = normalize(random_vec - N * dot(random_vec, N));
    vec3 B = cross(N, T);
    mat3 TBN = mat3(T, B, N);
    float occlusion = 0.0;
    for (int i = 0; i < kernel_size; i++)
    {
        vec3 sample_point = TBN * Samples[i].xyz;
        sample_point = pos + sample_point * radius;
        vec4 offset = vec4(sample_point, 1.0);
        offset = BBProjectionMatrix * offset;
        offset.xyz /= offset.w;
        offset.xyz = offset.xyz * 0.5 + 0.5;
        float sample_depth = -texture(PositionTex, offset.xy).w;
        float weight = smoothstep(0.0, 1.0, radius / abs(pos.z - sample_depth));
        occlusion += (sample_depth >= sample_point.z ? 1.0 : 0.0) * weight;
    }
    occlusion = 1.0 - (occlusion / kernel_size);

    vec3 albedo = texture(AlbedoTex, v2f_texcoord).xyz;
    FragColor = vec4(albedo * occlusion, 1.0);

	// Color_ = vec4(Albedo * Ambient + texture(u_SSDOTexture, v2f_TexCoords).rgb * 2,1);
}
