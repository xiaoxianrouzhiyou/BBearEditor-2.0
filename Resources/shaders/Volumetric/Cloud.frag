#version 330 core

in vec2 v2f_texcoord;
in mat4 v2f_inverse_projection_matrix;
in mat4 v2f_inverse_view_matrix;

out vec4 FragColor;

uniform sampler2D AlbedoTex;
uniform sampler2D NormalTex;
uniform sampler2D PositionTex;

uniform vec4 BBCameraParameters;
uniform vec4 BBCameraPosition;
uniform sampler2D BBCameraDepthTexture;

const int StepLength = 256;

float linearizeDepth(float depth)
{
    float z_near = BBCameraParameters.z;
    float z_far = BBCameraParameters.w;
    float z = depth * 2.0 - 1.0; 
    return (2.0 * z_near * z_far) / (z_far + z_near - z * (z_far - z_near));    
}

// compute the world pos of per pixel
vec3 computePixelWorldPos(vec2 uv, float depth)
{
    vec4 ndc_pos = vec4(2.0 * uv - 1.0, depth, 1.0);
    // -> view space
    vec4 view_space_pos = v2f_inverse_projection_matrix * ndc_pos;
    view_space_pos /= view_space_pos.w;
    // -> world space
    vec4 world_pos = v2f_inverse_view_matrix * view_space_pos;
    return world_pos.xyz;
}

float rayMarching(vec3 start, vec3 dir)
{
    vec3 pos = start;
    float sum = 0.0;
    vec3 step = dir * 0.05;
    for (int i = 0; i < StepLength; i++)
    {
        pos += step;
        if (pos.x < 1.0 && pos.x > -1.0 && pos.y < 1.0 && pos.y > -1.0 && pos.z < 1.0 && pos.z > -1.0)
        {
            sum += 0.01;
        }
    }
    return sum;
}


void main(void)
{
    vec3 final_color = vec3(1.0);
    vec3 albedo = texture(AlbedoTex, v2f_texcoord).rgb;

    float depth = texture(BBCameraDepthTexture, v2f_texcoord).r;
    depth = -linearizeDepth(depth);
    vec3 pixel_world_pos = computePixelWorldPos(v2f_texcoord, depth);
    vec3 ray_pos = BBCameraPosition.xyz;
    vec3 V = normalize(pixel_world_pos - ray_pos);

    float cloud = rayMarching(ray_pos, V);
    final_color *= cloud;

    final_color += albedo;

    FragColor = vec4(final_color, 1.0);
}

