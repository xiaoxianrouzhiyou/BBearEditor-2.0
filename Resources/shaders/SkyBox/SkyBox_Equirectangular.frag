#version 330 core

in vec3 v2f_local_pos;

layout (location = 0) out vec4 FragColor;

uniform sampler2D BBSkyBoxEquirectangularMap;

const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 calculateSampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}


void main(void)
{
    vec2 uv = calculateSampleSphericalMap(normalize(v2f_local_pos));
    FragColor = texture(BBSkyBoxEquirectangularMap, uv);
}
