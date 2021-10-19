#version 430 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in float v2g_view_space_particle_radius[];

out float g2f_view_space_particle_radius;
out vec3 g2f_view_space_center_pos;
out vec3 g2f_frag_pos;

uniform mat4 BBProjectionMatrix;

void main()
{
    g2f_view_space_particle_radius = v2g_view_space_particle_radius[0];
    g2f_view_space_center_pos = gl_in[0].gl_Position.xyz;

    g2f_frag_pos = g2f_view_space_center_pos + vec3(-g2f_view_space_particle_radius, -g2f_view_space_particle_radius, 0.0);
    gl_Position = BBProjectionMatrix * vec4(g2f_frag_pos, 1.0);
    EmitVertex();

    g2f_frag_pos = g2f_view_space_center_pos + vec3(g2f_view_space_particle_radius, -g2f_view_space_particle_radius, 0.0);
    gl_Position = BBProjectionMatrix * vec4(g2f_frag_pos, 1.0);
    EmitVertex();

    g2f_frag_pos = g2f_view_space_center_pos + vec3(-g2f_view_space_particle_radius, g2f_view_space_particle_radius, 0.0);
    gl_Position = BBProjectionMatrix * vec4(g2f_frag_pos, 1.0);
    EmitVertex();

    g2f_frag_pos = g2f_view_space_center_pos + vec3(g2f_view_space_particle_radius, g2f_view_space_particle_radius, 0.0);
    gl_Position = BBProjectionMatrix * vec4(g2f_frag_pos, 1.0);
    EmitVertex();

    EndPrimitive();
}