varying vec2 V_texcoord;
varying vec4 V_clip_space_pos;
varying float V_view_space_z;

uniform sampler2D BBCameraDepthTexture;

uniform vec4 BBColor_Water_Color01;
uniform vec4 BBColor_Water_Color02;
uniform sampler2D Foam;
uniform float FoamRange;
uniform float FoamNoise;
uniform vec4 BBColor_Foam_Color;

float computeLinearEyeDepth(float z, vec4 z_buffer_params)
{
    return 1.0 / (z_buffer_params.z * z + z_buffer_params.w);
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
    final_color = water_color;
    
    // water foam
    // 0~1 -> 0~5
    FoamRange *= 5.0;
    float foam_range = depth_water * FoamRange;
    float foam_tex = texture2D(Foam, V_texcoord).r;
    foam_tex = pow(foam_tex, FoamNoise);
    float foam_mask = step(foam_range, foam_tex);
    final_color += foam_mask * BBColor_Foam_Color;

    gl_FragColor = final_color;
}
