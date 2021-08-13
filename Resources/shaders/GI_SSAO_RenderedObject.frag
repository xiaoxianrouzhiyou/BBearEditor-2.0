#version 430 core

in vec2 v2f_texcoord;
in vec3 v2f_normal;
in vec3 v2f_view_space_pos;

layout (location = 0) out vec4 Albedo;
layout (location = 1) out vec4 Normal;
layout (location = 2) out vec4 Position;

uniform vec4 BBCameraParameters;
uniform sampler2D ObjectDiffuseTexture;
uniform vec4 BBLightColor;
uniform vec4 BBLightPosition;
uniform mat4 BBViewMatrix;

float linearizeDepth(float depth)
{
    float z_near = BBCameraParameters.z;
    float z_far = BBCameraParameters.w;
    float z = depth * 2.0 - 1.0; 
    return (2.0 * z_near * z_far) / (z_far + z_near - z * (z_far - z_near));    
}

void main(void)
{
    Position = vec4(v2f_view_space_pos, linearizeDepth(gl_FragCoord.z));
	Normal.xyz = normalize(v2f_normal);
    Normal.a = 1.0;
	vec3 view_space_light_pos = vec3(BBViewMatrix * BBLightPosition);
    vec3 L = vec3(0.0);
    float intensity = 0.0;
    if (BBLightPosition.w == 0.0)
    {
        // directional light
        L = normalize(BBLightPosition.xyz);
        intensity = max(dot(Normal.xyz, normalize(L)), 0.0);
    }
    else
    {
        L = view_space_light_pos - Position.xyz;
        intensity = max(dot(Normal.xyz, normalize(L)), 0.0) / dot(L, L) * 50.0;
    }
	// Albedo = texture(ObjectDiffuseTexture, v2f_texcoord).rgb * max(dot(Normal.xyz, normalize(L)), 0.0) / dot(L, L) * 50.0;
    Albedo = vec4(intensity, intensity, intensity, 1.0);
}
