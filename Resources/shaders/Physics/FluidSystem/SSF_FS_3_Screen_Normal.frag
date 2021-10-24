#version 430 core

in vec2 v2f_texcoords;

layout (location = 0) out vec4 FragColor;

uniform vec4 BBCameraParameters;
uniform vec4 BBCameraParameters1;

uniform sampler2D DepthMap;

const float epo = 1e-7;
float near;
float far;
float aspect;
float near_height;

// Clip space to view space
vec3 clip2view(vec2 texcoords, float depth)
{
    vec2 uv = (2.0 * texcoords - vec2(1.0, 1.0)) * vec2(aspect, 1.0);
    // near
    vec2 pos = 0.5 * near_height * uv * depth / near;
    return vec3(pos, -depth);
}

vec3 getViewSpacePos(vec2 texcoords)
{
    // DepthMap is linear depth
    float depth = texture(DepthMap, texcoords).r;
    return clip2view(texcoords, depth);
}

void main()
{
    near = BBCameraParameters.z;
    far = BBCameraParameters.w;
    aspect = BBCameraParameters1.y;
    near_height = BBCameraParameters1.z;

    float depth = texture(DepthMap, v2f_texcoords).r;
    if (depth >= far - 1.0) 
    {
        discard;
    }

    vec2 tex_size = vec2(1.0 / textureSize(DepthMap, 0).s, 1.0 / textureSize(DepthMap, 0).t);
    
    // current
    vec3 view_space_pos = clip2view(v2f_texcoords, depth);

    // Differentiation of u and v
    // Use two opposite directions for differentiation, and select the one with smaller absolute value
    // Because if a single differentiation is used, the normal will suddenly change at the edge of the object, resulting in errors
    vec3 du1 = getViewSpacePos(v2f_texcoords + vec2(tex_size.x, 0.0)) - view_space_pos;
    vec3 du2 = view_space_pos - getViewSpacePos(v2f_texcoords - vec2(tex_size.x, 0.0));
    vec3 du = abs(du1.z) < abs(du2.z) ? du1 : du2;

    vec3 dv1 = getViewSpacePos(v2f_texcoords + vec2(0.0, tex_size.y)) - view_space_pos;
    vec3 dv2 = view_space_pos - getViewSpacePos(v2f_texcoords - vec2(0.0, tex_size.y));
    vec3 dv = abs(dv1.z) < abs(dv2.z) ? dv1 : dv2;

    vec3 N = cross(du, dv);
    N = normalize(N);
    FragColor = vec4(N, 1.0);
}