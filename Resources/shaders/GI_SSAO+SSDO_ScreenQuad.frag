#version 430 core

in vec2 v2f_texcoord;

layout (location = 0) out vec4 FragColor;

uniform vec4 BBCameraParameters;
uniform sampler2D AlbedoTex;
uniform sampler2D NormalTex;
uniform sampler2D PositionTex;
uniform sampler2D NoiseTex;
uniform vec4 Samples[64];

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

    FragColor = vec4(pos, 1.0);
}
