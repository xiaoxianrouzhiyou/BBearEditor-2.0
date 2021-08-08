varying vec4 V_texcoord;
varying vec4 V_normal_uv;
varying vec4 V_world_space_pos;
varying vec4 V_world_space_normal;
varying vec4 V_clip_space_pos;
varying vec4 V_view_space_pos;
varying mat3 V_TBN;
varying mat4 V_viewMatrix_I;

uniform sampler2D BBCameraDepthTexture;
uniform sampler2D BBCameraColorTexture;
uniform vec4 BBLightPosition;
uniform vec4 BBCameraPosition;
uniform vec4 BBTime;


uniform vec4 BBColor_Water_Color01;
uniform vec4 BBColor_Water_Color02;

uniform sampler2D Foam;
uniform float FoamRange;
uniform float FoamNoise;
uniform vec4 BBColor_Foam_Color;

uniform sampler2D Normal_Map;
uniform float Distort;

uniform vec4 BBColor_Specular_Color;
uniform float Specular_Intensity;
uniform float Specular_Smoothness;

uniform samplerCube Reflection_Cube;

uniform sampler2D Caustics_Map;
uniform float Speed;


float computeLinearEyeDepth(float z, vec4 z_buffer_params)
{
    return 1.0 / (z_buffer_params.z * z + z_buffer_params.w);
}

vec3 decodeNormalMap(sampler2D normal_map, vec2 uv)
{
    vec3 normal = vec3(texture2D(normal_map, uv));
    // 0~1 -> -1~1
    normal = normal * 2.0 - vec3(1.0);
    normal = normalize(normal);
    normal = V_TBN * normal;
    normal = normalize(normal);
    return normal;
}

void main(void)
{
    // params
    float z_near = 0.1;
    float z_far = 1000.0;
    float zc0 = (1.0 - z_far / z_near) / 2.0;
    float zc1 = (1.0 + z_far / z_near) / 2.0;
    vec4 z_buffer_params = vec4(zc0, zc1, zc0 / z_far, zc1 / z_far);
    vec4 final_color = vec4(1.0);

    // water depth (static)
    vec2 screen_space_uv = V_clip_space_pos.xy / V_clip_space_pos.w * 0.5 + vec2(0.5);
    float depth_scene = texture2D(BBCameraDepthTexture, screen_space_uv).r;
    depth_scene = computeLinearEyeDepth(depth_scene, z_buffer_params);
    float depth_water = V_view_space_pos.z;
    depth_water *= 2.0;
    depth_water += depth_scene;

    // water color
    vec4 water_color = mix(BBColor_Water_Color01, BBColor_Water_Color02, depth_water);

    // water foam
    // 0~1 -> 0~5
    FoamRange *= 5.0;
    float foam_range = depth_water * FoamRange;
    float foam_tex = texture2D(Foam, V_texcoord.xy).r;
    foam_tex = pow(foam_tex, FoamNoise);
    float foam_mask = step(foam_range, foam_tex);
    // vec4 foam_color = foam_mask * BBColor_Foam_Color;

    // water distort
    vec3 normal_tex01 = decodeNormalMap(Normal_Map, V_normal_uv.xy);
    vec3 normal_tex02 = decodeNormalMap(Normal_Map, V_normal_uv.zw);
    vec3 normal_tex = normal_tex01 * normal_tex02;
    vec2 distort_uv = mix(screen_space_uv, normal_tex01.rr, Distort);
    // Remove the distortion effect of objects on the water
    float distort_depth_scene = texture2D(BBCameraDepthTexture, distort_uv).r;
    distort_depth_scene = computeLinearEyeDepth(distort_depth_scene, z_buffer_params);
    float distort_depth_water = V_view_space_pos.z;
    distort_depth_water *= 2.0;
    distort_depth_water += distort_depth_scene;
    vec2 final_distort_uv = distort_uv;
    if (distort_depth_water < 0)
    {
        // the region of objects on the water
        final_distort_uv = screen_space_uv;
    }
    vec4 distort_color = texture2D(BBCameraColorTexture, final_distort_uv);

    // Caustics
    vec4 view_space_depth = vec4(1.0);
    view_space_depth.xy = V_view_space_pos.xy * distort_depth_scene / (-V_view_space_pos.z);
    view_space_depth.z = distort_depth_scene;
    // Coordinates of points on the depth map in view space -> world space of current object
    vec4 world_space_depth = V_viewMatrix_I * view_space_depth;
    vec2 caustics_uv01 = world_space_depth.xz * 0.3 + vec2(0.0005) * Speed * BBTime.z;
    vec4 caustics_tex01 = texture2D(Caustics_Map, caustics_uv01);
    vec2 caustics_uv02 = world_space_depth.xz * 0.4 + vec2(-0.000535, 0.000675) * Speed * BBTime.z;
    vec4 caustics_tex02 = texture2D(Caustics_Map, caustics_uv02);
    vec4 caustics_tex = min(caustics_tex01, caustics_tex02);
    
    // Specular
    vec3 N = mix(normalize(V_world_space_normal.xyz), normal_tex, 0.5);
    vec3 L = normalize(BBLightPosition.xyz);
    vec3 V = normalize(BBCameraPosition.xyz - V_world_space_pos.xyz);
    vec3 H = normalize(L + V);
    float NdotH = dot(N, H);
    Specular_Smoothness *= 50;
    vec3 specular = BBColor_Specular_Color * Specular_Intensity * pow(clamp(dot(N, H), 0.0, 1.0), Specular_Smoothness);

    // Reflection
    vec3 reflection_uvw = reflect(-V, N);
    vec4 reflection_tex = textureCube(Reflection_Cube, reflection_uvw);
    // If the line of sight is perpendicular to the plane, the reflection effect is small
    float fresnel = 1 - clamp(dot(N, V), 0.0, 1.0);
    // Weaken reflection effect
    fresnel = pow(fresnel, 3);
    vec4 reflection = reflection_tex * fresnel;

    if (foam_mask == 0.0)
    {
        final_color.rgb = distort_color.rgb * water_color.rgb;
    }
    else
    {
        final_color.rgb = BBColor_Foam_Color.rgb;
    }
    final_color.rgb += specular;
    final_color.rgb += reflection.rgb;
    final_color.rgb += caustics_tex.rgb;

    gl_FragColor = final_color;
}
