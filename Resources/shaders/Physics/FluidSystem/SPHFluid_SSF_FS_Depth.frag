#version 430 core

in float g2f_view_space_particle_radius;
in vec3 g2f_view_space_center_pos;
in vec3 g2f_frag_pos;

out float LinearDepth;

uniform mat4 BBProjectionMatrix;

void main()
{
    // Cull to circle
    float r = distance(g2f_view_space_center_pos, g2f_frag_pos);
    if (r > g2f_view_space_particle_radius)
    {
        discard;
    }

    float depth = sqrt(g2f_view_space_particle_radius * g2f_view_space_particle_radius - r * r);
    float view_space_depth = depth + g2f_frag_pos.z;
    vec4 clip_space_pos = BBProjectionMatrix * vec4(view_space_depth, view_space_depth, view_space_depth, 1.0);
    gl_FragDepth = clip_space_pos.z / clip_space_pos.w * 0.5 + 0.5;

    LinearDepth = -view_space_depth;
}