#version 430 core

in vec2 v2f_texcoord;

layout (location = 0) out vec4 FragColor;

uniform sampler2D SSAOTex;
uniform sampler2D AlbedoTex;

void main(void)
{
    vec2 texel_size = 1.0 / vec2(textureSize(SSAOTex, 0));
    float result = 0.0;
    for (int x = -2; x < 2; x++) 
    {
        for (int y = -2; y < 2; y++) 
        {
            vec2 offset = vec2(float(x), float(y)) * texel_size;
            result += texture(SSAOTex, v2f_texcoord + offset).r;
        }
    }
    result = result / (4.0 * 4.0);

    vec3 albedo = texture(AlbedoTex, v2f_texcoord).xyz;
    FragColor = vec4(albedo * result, 1.0);
}
