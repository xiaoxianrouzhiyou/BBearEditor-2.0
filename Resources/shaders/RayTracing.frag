#version 430 core

in vec4 v2f_world_space_pos;
in vec2 v2f_texcoord;
in vec4 v2f_normal;

layout (location = 0) out vec4 FragColor;

uniform sampler2D BBCameraColorTexture;
uniform sampler2D BBCameraDepthTexture;
uniform vec4 BBCameraParameters;
uniform vec4 BBCameraPosition;
uniform mat4 BBProjectionMatrix;
uniform mat4 BBViewMatrix;
uniform sampler2D DiffuseTex;
uniform sampler2D DistortTex;
uniform float Distort;

float RayLength = 1000;
float stride = 1.0;

float linearizeDepth(float depth)
{
    float z_near = BBCameraParameters.z;
    float z_far = BBCameraParameters.w;
    float z = depth * 2.0 - 1.0; 
    return (2.0 * z_near * z_far) / (z_far + z_near - z * (z_far - z_near));    
}

struct Ray
{
    vec3 origin;
    vec3 direction;
};

struct Result
{
    bool is_hit;
    vec2 uv;
    vec3 position;
    int iteration_count;
};

vec4 projectToScreenSpace(vec3 point)
{
    return BBProjectionMatrix * vec4(point, 1.0);
}

vec3 projectToViewSpace(vec3 point)
{
    return vec3(BBViewMatrix * vec4(point, 1.0));
}

float distanceSquared(vec2 A, vec2 B)
{
    A = A - B;
    return dot(A, A);
}

bool query(vec2 z, vec2 uv)
{
    float depth = texture(BBCameraDepthTexture, uv / BBCameraParameters.xy).r;
    depth = -linearizeDepth(depth);
    // Before marching, it is before the depth value of the depth map
    // but after marching, it is in the back of depth value, indicating intersection
    return z.y < depth;
}

Result computeRayMarching(Ray ray)
{
    Result result;

    // world space
    vec3 begin = ray.origin;
    vec3 end = ray.origin + ray.direction * RayLength;

    // view space
    vec3 V0 = projectToViewSpace(begin);
    vec3 V1 = projectToViewSpace(end);
    
    // clip space
    vec4 H0 = projectToScreenSpace(V0);
    vec4 H1 = projectToScreenSpace(V1);

    float k0 = 1.0 / H0.w;
    float k1 = 1.0 / H1.w;

    vec3 Q0 = V0 * k0;
    vec3 Q1 = V1 * k1;

    // NDC space
    vec2 P0 = H0.xy * k0;
    vec2 P1 = H1.xy * k1;

    vec2 screen_size = BBCameraParameters.xy;

    // -1~1 -> 0~1
    P0 = (P0 + 1) / 2;
    P1 = (P1 + 1) / 2;
    // screen space
    P0 *= screen_size;
    P1 *= screen_size;
    // if the distance between P0 and P1 is too small
    P1 += vec2((distanceSquared(P0, P1) < 0.0001) ? 0.01 : 0.0);

    // DDA
    vec2 delta = P1 - P0;
    bool permute = false;
    if (abs(delta.x) < abs(delta.y))
    {
        permute = true;
        delta = delta.yx;
        P0 = P0.yx;
        P1 = P1.yx;
    }
    float step_dir = sign(delta.x);
    float dx = step_dir / delta.x;
    vec3 dQ = (Q1 - Q0) * dx;
    float dk = (k1 - k0) * dx;
    vec2 dP = vec2(step_dir, delta.y * dx);
    dP *= stride;
    dQ *= stride;
    dk *= stride;
    P0 += dP;
    Q0 += dQ;
    k0 += dk;

    int step = 0;
    int max_step = 5000;
    float k = k0;
    vec3 Q = Q0;
    float prev_z = V0.z;
    for (vec2 P = P0; step < max_step; step++, P += dP, Q.z += dQ.z, k += dk)
    {
        result.uv = permute ? P.yx : P;
        vec2 depths;
        // origin z
        depths.x = prev_z;
        // z after marching
        depths.y = (dQ.z * 0.5 + Q.z) / (dk * 0.5 + k);

        prev_z = depths.y;

        if (depths.x < depths.y)
            depths.xy = depths.yx;

        if (result.uv.x > BBCameraParameters.x || result.uv.x < 0.0 || result.uv.y > BBCameraParameters.y || result.uv.y < 0.0)
            break;

        result.is_hit = query(depths, result.uv);
        if (result.is_hit)
            break;
    }

    return result;
}

void main(void)
{
    vec3 origin_point = v2f_world_space_pos.xyz;
    vec3 view_dir = normalize(BBCameraPosition.xyz - origin_point);
    // vec3 normal = normalize(v2f_normal.xyz);
    vec3 normal = vec3(0.0, 1.0, 0.0);
    vec3 reflect_dir = normalize(reflect(-view_dir, normal));

    Ray ray;
    ray.origin = origin_point;
    ray.direction = reflect_dir;
    Result result = computeRayMarching(ray);

    if (result.is_hit)
    {
        vec2 uv = result.uv / BBCameraParameters.xy;
        vec4 distort_color = texture2D(DistortTex, v2f_texcoord.xy);
        uv = mix(uv, distort_color.xy, Distort);
        // Reflected color
        FragColor = texture(BBCameraColorTexture, uv);
    }
    else
    {
        // original color
        vec4 screen_space_pos = BBProjectionMatrix * BBViewMatrix * v2f_world_space_pos;
        screen_space_pos.xy /= screen_space_pos.w;
        // -1~1 -> 0~1
        screen_space_pos.xy = screen_space_pos.xy * 0.5 + 0.5;
        FragColor = texture(BBCameraColorTexture, screen_space_pos.xy);
    }
}
