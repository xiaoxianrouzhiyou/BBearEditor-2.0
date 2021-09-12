#version 330 core

in vec2 v2f_texcoord;
in mat4 v2f_inverse_projection_matrix;
in mat4 v2f_inverse_view_matrix;

out vec4 FragColor;

uniform sampler2D AlbedoTex;
uniform sampler2D NormalTex;
uniform sampler2D PositionTex;
uniform sampler3D NoiseTex;

uniform vec4 BBCameraPosition;
uniform sampler2D BBCameraDepthTexture;

const vec3 BoundingBoxMin = vec3(-2, -2, -2);
const vec3 BoundingBoxMax = vec3(2, 2, 2);
const int Loop = 256;


// compute the world pos of per pixel
vec3 computePixelWorldPos(vec2 uv, float depth)
{
    vec4 ndc_pos = vec4(uv * 2.0 - 1.0, depth * 2.0 - 1.0, 1.0);
    // -> view space
    vec4 view_space_pos = v2f_inverse_projection_matrix * ndc_pos;
    // -> world space
    vec4 world_pos = v2f_inverse_view_matrix * view_space_pos;
    world_pos /= world_pos.w;
    return world_pos.xyz;
}

// A Ray-Box Intersection Algorithm and Efficient Dynamic Voxel Rendering
// http://jcgt.org/published/0007/03/04/
// ray_origin_pos : camera pos
// inv_ray_dir : inverse ray direction
vec2 rayToContainer(vec3 box_min, vec3 box_max, vec3 ray_origin_pos, vec3 inv_ray_dir)
{
    vec3 t0 = (box_min - ray_origin_pos) * inv_ray_dir;
    vec3 t1 = (box_max - ray_origin_pos) * inv_ray_dir;
    vec3 tmin = min(t0, t1);
    vec3 tmax = max(t0, t1);

    // Entering point
    float enter = max(max(tmin.x, tmin.y), tmin.z);
    // Leaving point
    float leave = min(tmax.x, min(tmax.y, tmax.z));

    // The distance between camera and container 
    float camera_to_container = max(0, enter);
    // The distance the light travels within the bounding box
    float travel_distance = max(0, leave - camera_to_container);

    return vec2(camera_to_container, travel_distance);
}

float sampleDensity(vec3 ray_pos) 
{
    vec3 uvw = ray_pos;
    return texture(NoiseTex, uvw).r;
}

float rayMarching(vec3 enter, vec3 dir, float distance_limit)
{
    vec3 ray_pos = enter;
    float sum_density = 0.0;
    float displacement = 0.0;
    float step = 0.1;

    for (int i = 0; i < Loop; i++)
    {
        if (displacement < distance_limit)
        {
            ray_pos = enter + dir * displacement;
            sum_density += pow(sampleDensity(ray_pos), 5.0);
        }
        displacement += step;
    }

    return sum_density;
}


void main(void)
{
    vec3 final_color = vec3(1.0);
    vec3 albedo = texture(AlbedoTex, v2f_texcoord).rgb;

    float depth = texture(BBCameraDepthTexture, v2f_texcoord).r;
    vec3 pixel_world_pos = computePixelWorldPos(v2f_texcoord, depth);
    vec3 ray_pos = BBCameraPosition.xyz;
    vec3 camera_to_object = pixel_world_pos - ray_pos;
    vec3 V = normalize(camera_to_object);

    // Compute ray to the container surrounding the cloud
    // Raymarching is started at the intersection with the cloud container
    float camera_to_object_distance = length(camera_to_object);
    vec2 ray_to_container_result = rayToContainer(BoundingBoxMin, BoundingBoxMax, ray_pos, vec3(1.0) / V);
    float camera_to_container = ray_to_container_result.x;
    float travel_distance = ray_to_container_result.y;
    // When distance_limit < 0, we do not need to compute
    float distance_limit = min(camera_to_object_distance - camera_to_container, travel_distance);
    // the intersection with the cloud container
    vec3 enter = ray_pos + V * camera_to_container;
    float density = rayMarching(enter, V, distance_limit);
    final_color *= density;

    final_color += albedo;

    FragColor = vec4(final_color, 1.0);
}

