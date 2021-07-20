varying vec4 V_Position;
varying vec4 V_Texcoord;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture3;
uniform mat4 projectionMatrix_I;
uniform mat4 viewMatrix_I;

vec4 UV2PointOnPlane(vec4 point)
{
    point = 2 * point;
    point = point - vec4(1.0);
    point = viewMatrix_I * projectionMatrix_I * point;
    point = vec4(point.xyz / point.w, 1.0);
    return point;
}

bool hitObject(vec2 coord, float min_distance, float max_distance, out vec2 hit_point)
{
    vec3 model_normal = texture2D(texture1, coord);
    return length(model_normal) != 0.0;
}

vec3 hash3(vec2 coord)
{
    vec3 rand = vec3(dot(coord, vec2(127.1, 311.7)), dot(coord, vec2(269.5, 183.3)), dot(coord, vec2(419.2, 371.9)));
    rand = fract(sin(rand) * 43758.5453);
    rand = normalize(rand);
    return rand;
}

vec3 getWorldPosition(vec2 coord)
{
    return texture2D(texture0, coord);
}

vec3 getObjectNormal(vec2 coord)
{
    return texture2D(texture1, coord);
}

vec3 getBackGroundColor(vec2 coord)
{
    return texture2D(texture3, coord);
}

vec3 scatter(vec2 coord, vec3 input_ray)
{
    vec3 rand_direction = hash3(coord);
    vec3 new_direction = getObjectNormal(coord) + rand_direction;
    return new_direction;
}

vec3 getColor(vec2 coord, vec3 input_ray)
{
    vec2 hit_point;
    if (hitObject(coord, 0.0, 100.0, hit_point))
    {
        vec3 scatter_ray = scatter(coord, input_ray);
        return getColor(hit_point, scatter_ray);
    }
    return getBackGroundColor(coord);
}

void main(void)
{
    vec3 final_color = vec3(0.0);

    vec4 point_near_plane = UV2PointOnPlane(vec4(V_Texcoord.xy, 0.0, 1.0));
    vec4 point_far_plane = UV2PointOnPlane(vec4(V_Texcoord.xy, 1.0, 1.0));

    vec3 ray_direction = normalize(point_far_plane.xyz - point_near_plane.xyz);

    if (ray_direction.x < 0.0)
        ray_direction.x = -ray_direction.x;
    if (ray_direction.y < 0.0)
        ray_direction.y = -ray_direction.y;
    if (ray_direction.z < 0.0)
        ray_direction.z = -ray_direction.z;

    vec3 model_normal = texture2D(texture1, V_Texcoord.xy);
    // 0~1
    model_normal = model_normal * 0.5 + 0.5;

    gl_FragColor = vec4(getWorldPosition(V_Texcoord.xy), 1.0);
}
