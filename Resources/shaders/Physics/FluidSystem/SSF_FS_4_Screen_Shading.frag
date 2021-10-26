#version 430 core

in vec2 v2f_texcoords;

layout (location = 0) out vec4 FragColor;

uniform vec4 BBCameraParameters;
uniform vec4 BBCameraParameters1;
uniform vec4 BBLightPosition;
uniform vec4 BBLightColor;
uniform mat4 BBViewMatrix;
uniform samplerCube BBSkyBox;
uniform sampler2D BBCameraColorTexture;

uniform sampler2D DepthMap;
uniform sampler2D ThicknessMap;
uniform sampler2D NormalMap;


const vec3 WaterRefractedColor = vec3(0.05f, 0.5f, 0.8f);
const vec3 WaterF0 = vec3(0.15f);
const float WaterK = 0.05f;
const float RefractedScale = 2.0f;
const float epo = 1e-2;
float near;
float far;
float aspect;
float near_height;


// Clip space to view space
vec3 clip2view(vec2 texcoords, float depth)
{
    vec2 uv = (2.0 * texcoords - vec2(1.0, 1.0)) * vec2(aspect, 1.0);
    // near
    vec2 pos = 0.5 * near_height * uv * depth / near;
    return vec3(pos, -depth);
}

vec3 computeFresnel(vec3 F0, float cos_theta)
{
    return F0 + (1.0 - F0) * pow(1.0 - cos_theta, 5.0);
}


void main()
{
    near = BBCameraParameters.z;
    far = BBCameraParameters.w;
    aspect = BBCameraParameters1.y;
    near_height = BBCameraParameters1.z;

    vec3 L = normalize(mat3(BBViewMatrix) * BBLightPosition.xyz);
    float depth = texture(DepthMap, v2f_texcoords).r;

    if (depth >= far - epo)
        discard;

    vec2 tex_size = vec2(1.0 / textureSize(NormalMap, 0).s, 1.0 / textureSize(NormalMap, 0).t);
    vec3 N = texture(NormalMap, v2f_texcoords).xyz;
    float thickness = texture(ThicknessMap, v2f_texcoords).r;

    // phong
    vec3 ambient = BBLightColor.rgb * 0.1;
    vec3 diffuse = ambient + BBLightColor.rgb * max(dot(N, L), 0.0);
    
    // blinn-phong
    vec3 view_pos = clip2view(v2f_texcoords, depth);
    vec3 V = normalize(-view_pos);
    vec3 H = normalize(V + L);
    vec3 specular = BBLightColor.rgb * pow(max(dot(N, H), 0.0), 30.0);
    
    // fresnel
    vec3 fresnel = computeFresnel(WaterF0, max(dot(N, V), 0.0));

    // world space
    vec3 reflected = normalize(mat3(inverse(BBViewMatrix) * reflect(-V, N)));   
    vec3 reflected_sky = textureCube(BBSkyBox, reflected).rgb;

    // refraction
    vec3 refracted = texture(BBCameraColorTexture, v2f_texcoords - N.xy * thickness / far * RefractedScale).xyz;

    // transparency
    float transparency = exp(-thickness * WaterK);
    reflected = mix(WaterRefractedColor, refracted, transparency);

    FragColor = vec4(mix(refracted, reflected_sky, fresnel), 1.0);
}