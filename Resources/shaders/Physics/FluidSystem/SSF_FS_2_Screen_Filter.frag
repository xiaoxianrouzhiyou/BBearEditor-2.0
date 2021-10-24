#version 430 core

in vec2 v2f_texcoords;

layout (location = 0) out float Depth;
layout (location = 1) out float Thickness;

uniform vec4 BBCameraParameters;

uniform sampler2D DepthMap;
uniform sampler2D ThicknessMap;
uniform float FilterRadius;
uniform float SpatialScale;
uniform float RangeScale;
uniform vec4 BlurDir;


void computeGaussianFilter(vec2 blur_dir, float depth)
{
    float sum_depth = 0.0;
    float w_sum_depth = 0.0;
    float sum_thickness = 0.0;
    float w_sum_thickness = 0.0;
    for (float x = -FilterRadius; x <= FilterRadius; x += 1.0)
    {
        float sample_depth = texture(DepthMap, v2f_texcoords + x * blur_dir.xy).r;
        float sample_thickness = texture(ThicknessMap, v2f_texcoords + x * blur_dir.xy).r;

        // Spatial weight
        float s = x * SpatialScale;
        s = exp(-s * s);

        // Range weight
        float r = (sample_depth - depth) * RangeScale;
        r = exp(-r * r);

        // Gaussian filter thickness
        sum_thickness += sample_thickness * s;
        w_sum_thickness += s;

        // Bilateral filter depth, Consider spatial & range, 
        // The value with large depth difference can still save its original depth information after bilateral filtering
        sum_depth += sample_depth * s * r;
        w_sum_depth += s * r;
    }
    Depth = sum_depth / w_sum_depth;
    Thickness = sum_thickness / w_sum_thickness;
}

void main()
{
    vec2 depth_map_size = textureSize(DepthMap, 0);
    vec2 thickness_map_size = textureSize(ThicknessMap, 0);

    float depth = texture(DepthMap, v2f_texcoords).r;

    float viewport_width = BBCameraParameters.x;
    float viewport_height = BBCameraParameters.y;
    vec2 blur_dir = BlurDir.xy * vec2(1.0 / viewport_width, 1.0 / viewport_height) * 1.0;
    computeGaussianFilter(blur_dir, depth);
}