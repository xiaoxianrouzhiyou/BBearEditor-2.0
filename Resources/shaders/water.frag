varying vec4 V_clip_space_pos;
varying float V_view_space_z;

uniform sampler2D BBCameraDepthTexture;

float computeLinearEyeDepth(float z, vec4 z_buffer_params)
{
    return 1.0 / (z_buffer_params.z * z + z_buffer_params.w);
}

void main(void)
{
    float z_near = 0.1;
    float z_far = 1000.0;
    float zc0 = (1.0 - z_far / z_near) / 2.0;
    float zc1 = (1.0 + z_far / z_near) / 2.0;
    vec4 z_buffer_params = vec4(zc0, zc1, zc0 / z_far, zc1 / z_far);

    vec2 screen_space_uv = V_clip_space_pos.xy / V_clip_space_pos.w * 0.5 + vec2(0.5);
    float depth_scene = texture2D(BBCameraDepthTexture, screen_space_uv).r;
    depth_scene = computeLinearEyeDepth(depth_scene, z_buffer_params);

    float depth_water = V_view_space_z;
    depth_water *= 2.0;
    depth_water += depth_scene;
    
    gl_FragColor = vec4(depth_water, depth_water, depth_water, 1.0);
}
