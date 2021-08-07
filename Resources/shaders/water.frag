varying vec4 V_texcoord;
varying vec4 V_normal_uv;
varying vec4 V_world_space_pos;
varying vec4 V_world_space_normal;
varying vec4 V_clip_space_pos;
varying float V_view_space_z;
varying mat3 V_TBN;

uniform sampler2D BBCameraDepthTexture;
uniform sampler2D BBCameraColorTexture;
uniform vec4 BBLightPosition;
uniform vec4 BBCameraPosition;


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

    // water depth
    vec2 screen_space_uv = V_clip_space_pos.xy / V_clip_space_pos.w * 0.5 + vec2(0.5);
    float depth_scene = texture2D(BBCameraDepthTexture, screen_space_uv).r;
    depth_scene = computeLinearEyeDepth(depth_scene, z_buffer_params);
    float depth_water = V_view_space_z;
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
    float distort_depth_water = V_view_space_z;
    distort_depth_water *= 2.0;
    distort_depth_water += distort_depth_scene;
    vec2 final_distort_uv = distort_uv;
    if (distort_depth_water < 0)
    {
        // the region of objects on the water
        final_distort_uv = screen_space_uv;
    }
    vec4 distort_color = texture2D(BBCameraColorTexture, final_distort_uv);
    
    // Specular
    vec3 N = normal_tex;
    vec3 L = normalize(BBLightPosition.xyz);
    vec3 V = normalize(BBCameraPosition.xyz - V_world_space_pos.xyz);
    vec3 H = normalize(L + V);
    float NdotH = dot(N, H);
    Specular_Smoothness *= 50;
    vec3 specular = BBColor_Specular_Color * Specular_Intensity * pow(clamp(dot(N, H), 0.0, 1.0), Specular_Smoothness);



    if (foam_mask == 0.0)
    {
        final_color.rgb = distort_color.rgb * water_color.rgb;
    }
    else
    {
        final_color.rgb = BBColor_Foam_Color.rgb;
    }
    final_color.rgb += specular;

    gl_FragColor = final_color;
}
