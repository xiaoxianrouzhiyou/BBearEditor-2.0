#version 330 core

in vec2 v2f_texcoord;
in mat4 v2f_inverse_projection_matrix;
in mat4 v2f_inverse_view_matrix;

out vec4 FragColor;

uniform sampler2D AlbedoTex;
uniform sampler2D NormalTex;
uniform sampler2D PositionTex;
uniform sampler2D WeatherTex;
uniform sampler2D PerlinNoiseTex2D;
uniform sampler3D PerlinNoiseTex3D;
uniform sampler2D DistortTex;

uniform vec4 BBCameraPosition;
uniform sampler2D BBCameraDepthTexture;
uniform vec4 BBLightColor;
uniform vec4 BBLightPosition;
uniform vec4 BBTime;

const vec3 BoundingBoxMin = vec3(-8, 0, -8);
const vec3 BoundingBoxMax = vec3(8, 2, 8);
const int RayMarchingNum = 256;
const int TransmitNum = 8;
const vec3 Color0 = vec3(0.94, 0.97, 1.0);
const vec3 Color1 = vec3(0.94, 0.97, 1.0);
const float ColorOffset0 = 0.6;
const float ColorOffset1 = 1.0;
const float DarknessThreshold = 0.05; 
const vec4 PhaseParams = vec4(0.72, 1.0, 0.5, 1.58);
const vec4 NoiseWeights = vec4(-0.1, 30.0, -0.3, 1.0);
const float ContainerEdgeFadeDst = 5.0;

vec3 g_box_size;
vec3 g_box_center;


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
// case 1: 0 <= enter <= leave
// case 2: enter < 0 < leave
// case 3: enter > leave
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

float remap(float origin_val, float origin_min, float origin_max, float new_min, float new_max)
{
    return new_min + (origin_val - origin_min) / (origin_max - origin_min) * (new_max - new_min);
}

float sampleDensity(vec3 ray_pos) 
{
    vec3 offset = vec3(0.001, 0.0002, 0.0) * BBTime.z;

    vec3 noise_tex_uvw = ray_pos * 0.4 + offset;
    // The disturbances in different directions are different
    float noise_val = dot(texture(PerlinNoiseTex3D, noise_tex_uvw), normalize(NoiseWeights));

    vec2 weather_tex_uv = (ray_pos.xz - g_box_center.xz + g_box_size.xz * 0.5f) / max(g_box_size.x, g_box_size.z) + offset.xy * vec2(0.2, 0.2);
    // uv distortion
    vec4 distort = texture(DistortTex, v2f_texcoord);
    weather_tex_uv = mix(weather_tex_uv, distort.xy, 0.1);
    // Upper narrow and lower wide
    float weather_val = texture(WeatherTex, weather_tex_uv).r;

    float h_percent = (ray_pos.y - BoundingBoxMin.y) / g_box_size.y;
    // As the height rises, the gradient gradually flattens from 1
    float h_gradient = clamp(remap(h_percent, 0.0, weather_val, 1.0, 0.0), 0.0, 1.0);

    // Bottom fade
    float bottom = remap(weather_val, 0, 1, 0.1, 0.6);
    h_gradient *= clamp(remap(h_percent, 0.0, bottom, 0.0, 1.0), 0.0, 1.0);

    // Edge fade
    float dst_to_edge_x = min(ContainerEdgeFadeDst, min(ray_pos.x - BoundingBoxMin.x, BoundingBoxMax.x - ray_pos.x));
    float dst_to_edge_z = min(ContainerEdgeFadeDst, min(ray_pos.z - BoundingBoxMin.z, BoundingBoxMax.z - ray_pos.z));
    float edge_weight = min(dst_to_edge_x, dst_to_edge_z) / ContainerEdgeFadeDst;
    h_gradient *= edge_weight;

    return noise_val * h_gradient;
}

vec3 transmit(vec3 ray_pos, vec3 L, float displacement)
{
    float travel_distance = rayToContainer(BoundingBoxMin, BoundingBoxMax, ray_pos, vec3(1.0) / L).y;
    float step = travel_distance / TransmitNum;
    float sum_density = 0.0;

    for (int i = 0; i < TransmitNum; i++)
    {
        ray_pos += L * step;
        sum_density += max(0, sampleDensity(ray_pos));
    }
    float transmittance = exp(-sum_density * step);

    // color level : light color / color0 / color1
    vec3 cloud_color = mix(Color0, BBLightColor.rgb, clamp(transmittance * ColorOffset0, 0.0, 1.0));
    cloud_color = mix(Color1, cloud_color, clamp(pow(transmittance * ColorOffset1, 1), 0.0, 1.0));
    return DarknessThreshold + transmittance * (1 - DarknessThreshold) * cloud_color;
}

// HG phase function
float hg(float a, float g) 
{
    float g2 = g * g;
    return (1 - g2) / (4 * 3.1415 * pow(1 + g2 - 2 * g * (a), 1.5));
}

float phase(float a) 
{
    float blend = 0.5;
    float hg_blend = hg(a, PhaseParams.x) * (1 - blend) + hg(a, -PhaseParams.y) * blend;
    return PhaseParams.z + hg_blend * PhaseParams.w;
}

vec4 rayMarching(vec3 enter, vec3 V, float distance_limit, vec3 L)
{
    vec3 ray_pos = enter;
    vec3 light_energy = vec3(0.0);
    float sum_density = 1.0;
    // The distance that ray travels
    float displacement = 0.0;
    float step = 0.05;
    // The scattering towards the light is stronger
    // The edges of the cloud appear black
    float cos_angle = dot(V, L);
    vec3 phase_val = vec3(phase(cos_angle));

    // Cloud shape
    g_box_size = BoundingBoxMax - BoundingBoxMin;
    g_box_center = (BoundingBoxMax + BoundingBoxMin) * 0.5;

    for (int i = 0; i < RayMarchingNum; i++)
    {
        if (displacement < distance_limit)
        {
            ray_pos = enter + V * displacement;
            float density = sampleDensity(ray_pos);
            if (density > 0.0)
            {
                vec3 transmittance = transmit(ray_pos, L, displacement);
                light_energy += density * step * sum_density * transmittance * phase_val;
                // Beer-Lambert Law
                // The transmission intensity decreases exponentially with the increase of the propagation distance in the medium
                sum_density *= exp(-density * step);
                if (sum_density < 0.01)
                {
                    break;
                }
            }
        }
        displacement += step;
    }

    return vec4(light_energy, sum_density);
}


void main(void)
{
    vec3 albedo = texture(AlbedoTex, v2f_texcoord).rgb;

    float depth = texture(BBCameraDepthTexture, v2f_texcoord).r;
    vec3 pixel_world_pos = computePixelWorldPos(v2f_texcoord, depth);
    vec3 ray_pos = BBCameraPosition.xyz;
    vec3 camera_to_object = pixel_world_pos - ray_pos;
    vec3 V = normalize(camera_to_object);
    vec3 L = normalize(BBLightPosition.xyz);

    // Compute ray to the container surrounding the cloud
    // Raymarching is started at the intersection with the cloud container
    float camera_to_object_distance = length(camera_to_object);
    vec2 ray_to_container_result = rayToContainer(BoundingBoxMin, BoundingBoxMax, ray_pos, vec3(1.0) / V);
    float camera_to_container = ray_to_container_result.x;
    float travel_distance = ray_to_container_result.y;
    // When distance_limit < 0, we do not need to compute
    float distance_limit = min(camera_to_object_distance - camera_to_container, travel_distance);
    // The intersection with the cloud container
    vec3 enter = ray_pos + V * camera_to_container;
    vec4 ray_marching_result = rayMarching(enter, V, distance_limit, L);

    FragColor = 1 - vec4(0.06, 0.03, 0.0, ray_marching_result.a - 0.2);
}

