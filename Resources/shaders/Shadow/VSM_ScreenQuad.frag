#version 430 core

in vec2 v2f_texcoord;

uniform sampler2D AlbedoAndMetallicTex;
uniform sampler2D NormalAndDoubleRoughnessTex;
uniform sampler2D PositionTex;
uniform sampler2D BBShadowMap;

uniform vec4 BBCameraParameters;
uniform mat4 BBViewMatrix;
uniform mat4 BBViewMatrix_I;
uniform vec4 BBLightPosition;
uniform vec4 BBLightColor;
uniform mat4 BBLightProjectionMatrix;
uniform mat4 BBLightViewMatrix;


float linearizeDepth(float depth)
{
    float z_near = BBCameraParameters.z;
    float z_far = BBCameraParameters.w;
    float z = depth * 2.0 - 1.0; 
    return (2.0 * z_near * z_far) / (z_far + z_near - z * (z_far - z_near));    
}

void main(void)
{
    vec4 albedo = texture(AlbedoAndMetallicTex, v2f_texcoord);
    vec3 normal = texture(NormalAndDoubleRoughnessTex, v2f_texcoord).xyz;
    vec4 world_space_pos = texture(PositionTex, v2f_texcoord);

    if ((abs(normal.x) < 0.0001f) && (abs(normal.y) < 0.0001f) && (abs(normal.z) < 0.0001f))
    {
        gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    }

    normal = normalize(normal);
    vec4 light_space_pos = BBLightProjectionMatrix * BBLightViewMatrix * world_space_pos;
    light_space_pos /= 10.0;
    // 0~1
    light_space_pos.xyz = light_space_pos.xyz * vec3(0.5f) + vec3(0.5f);
    float current_depth = light_space_pos.z;

    // the position of directional light is dir
    vec3 view_space_light_dir = -normalize(vec3(BBViewMatrix * BBLightPosition));

    // compute mean, E, by filtering
    vec2 texel_size = 1.0 / textureSize(BBShadowMap, 0);

    vec2 E = vec2(0.0f);
    for (int y = -1; y <= 1; y++)
    {
        for (int x = -1; x <= 1; x++)
        {
            E += texture(BBShadowMap, light_space_pos.xy + texel_size * vec2(x, y)).rg;
        }
    }
    E /= 9.0f;
    // E(d)
    float Ed = E.x;
    // E(d^2)
    float Ed2 = E.y;
    float variance = Ed2 - Ed * Ed;

    float shadow = 0.0f;
    if (current_depth - 0.00001 <= Ed)
    {
        shadow = 1.0f;
    }
    else
    {
        shadow = variance / (variance + pow(current_depth - Ed, 2.0f));
    }
    float final_color = max(dot(view_space_light_dir, normal), 0.0) * shadow;

    gl_FragColor = vec4(final_color, final_color, final_color, 1.0);
}