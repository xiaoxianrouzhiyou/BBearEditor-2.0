#version 330 core
#extension GL_NV_shadow_samplers_cube : enable

in vec3 v2f_local_pos;

layout (location = 0) out vec4 FragColor;

uniform samplerCube EnvironmentMap;

const float PI = 3.14159265359;

void main(void)
{
    vec3 N = normalize(v2f_local_pos);

    vec3 irradiance = vec3(0.0);   
    
    // tangent space calculation from origin point
    vec3 up = vec3(0.0, 1.0, 0.0);
    vec3 right = normalize(cross(up, N));
    up = normalize(cross(N, right));

    // We traverse the hemisphere with a fixed sampledelta increment value.
    // Reducing (or increasing) this increment will increase (or decrease) the accuracy
    float sample_delta = 0.025;
    float sample_count = 0.0;
    for (float phi = 0.0; phi < 2.0 * PI; phi += sample_delta)
    {
        for (float theta = 0.0; theta < 0.5 * PI; theta += sample_delta)
        {
            // spherical to cartesian (in tangent space)
            vec3 tangent_sample = vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
            // tangent space to world
            vec3 sample_vec = tangent_sample.x * right + tangent_sample.y * up + tangent_sample.z * N; 

            // Use this vector to sample HDR environment map
            // cos(theta) : Light at larger angles is weaker
            // sin(theta) : The hemisphere's discrete sample area gets smaller the higher the zenith angle θ as the sample regions converge towards the center top.
            // sin(theta) : To compensate for the smaller areas, we weigh its contribution by scaling the area by sinθ.
            irradiance += texture(EnvironmentMap, sample_vec).rgb * cos(theta) * sin(theta);
            sample_count++;
        }
    }
    // average value
    irradiance = PI * irradiance * (1.0 / float(sample_count));
    
    FragColor = vec4(irradiance, 1.0);
}