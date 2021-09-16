#version 430 core

in vec2 v2f_texcoord;

out vec4 FragColor;

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
    vec3 albedo = texture(AlbedoAndMetallicTex, v2f_texcoord).rgb;
    vec3 normal = texture(NormalAndDoubleRoughnessTex, v2f_texcoord).xyz;
    vec4 world_space_pos = texture(PositionTex, v2f_texcoord);

    if ((abs(normal.x) < 0.0001f) && (abs(normal.y) < 0.0001f) && (abs(normal.z) < 0.0001f))
    {
        FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    }

    normal = normalize(normal);
    vec4 light_space_pos = BBLightProjectionMatrix * BBLightViewMatrix * world_space_pos;
    // light_space_pos.xyz /= 10.0;
    // 0~1
    light_space_pos.xyz = light_space_pos.xyz * vec3(0.5f) + vec3(0.5f);
    float current_depth = light_space_pos.z;

    // the position of directional light is dir
    vec3 view_space_light_dir = normalize(vec3(BBViewMatrix * BBLightPosition));

    // get mean, E, by mipmap
    vec2 E = textureLod(BBShadowMap, light_space_pos.xy, 2).rg;
    // E(d)
    float Ed = E.x;
    // E(d^2)
    float Ed2 = E.y;
    float variance = Ed2 - Ed * Ed;

    float visibility = 0.0f;
    if (current_depth - 0.0001 < Ed)
    {
        visibility = 1.0f;
    }
    else
    {
        visibility = variance / (variance + pow(current_depth - Ed, 2.0f));
    }
    float final_color = max(dot(view_space_light_dir, normal), 0.0) * visibility;

    FragColor = vec4(albedo * final_color, 1.0);
}