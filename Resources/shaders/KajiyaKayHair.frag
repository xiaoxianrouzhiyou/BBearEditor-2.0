varying vec4 V_texcoord;
varying vec4 V_world_space_tangent;
varying vec4 V_world_space_bitangent;
varying vec4 V_view_dir;

uniform vec4 BBLightPosition;

void main(void)
{
    vec3 T = normalize(V_world_space_tangent.xyz);
    vec3 B = normalize(V_world_space_bitangent.xyz);
    vec3 L = normalize(BBLightPosition.xyz);
    vec3 V = normalize(V_view_dir.xyz);
    vec3 H = normalize(L + V);

    float TdotH = dot(B, H);
    float sinTH = sqrt(1 - TdotH * TdotH);
    sinTH = clamp(sinTH, 0.0, 1.0);
    float specular = pow(sinTH, 60);

    gl_FragColor = vec4(specular, specular, specular, 1.0);
}
