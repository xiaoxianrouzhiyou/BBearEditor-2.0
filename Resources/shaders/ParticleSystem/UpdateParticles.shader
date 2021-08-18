#version 430

struct Vertex
{
    vec4 BBPosition;
    vec4 BBColor;
    vec4 BBTexcoord;
    vec4 BBNormal;
    vec4 BBTangent;
    vec4 BBBiTangent;
};

layout (std140, binding = 0) buffer Bundle
{
    Vertex vertexes[];
} bundle;

layout (local_size_x = 256, local_size_y = 1, local_size_z = 1) in;

uniform sampler2D NoiseTex;

vec3 computeV(vec3 pos)
{
    vec3 v = vec3(0.0);
    float gain = 1.0;
    float expend = 1.0;
    for (int i = 0; i < 4; i++)
    {
        v += texture(NoiseTex, vec2((pos * expend) / 30.0)).xyz * gain;
        expend *= 2.0;
        gain *= 0.5;
    }
    // return v;
    return vec3(1);
}

void main()
{
    // An effective three-dimensional index of the location of the current execution unit in the global workgroup
    // linear
    uint particle_id = gl_GlobalInvocationID.x;
    // 2 ^ 20
    if (particle_id > 1048576)
        return;
    vec3 pos = bundle.vertexes[particle_id].BBPosition.xyz;
    vec3 v = bundle.vertexes[particle_id].BBNormal.xyz;

    v += computeV(pos * 10.0) * 0.001;

    pos += v;
    // Slow down
    v *= 0.95;

    bundle.vertexes[particle_id].BBPosition = vec4(pos, 1.0);
    bundle.vertexes[particle_id].BBNormal = vec4(v, 1.0);
}