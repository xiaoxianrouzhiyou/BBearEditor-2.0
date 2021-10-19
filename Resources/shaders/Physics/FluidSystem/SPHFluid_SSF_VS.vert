#version 430 core

in vec4 BBPosition;

out float v2g_view_space_particle_radius;

uniform mat4 BBViewMatrix;
uniform mat4 BBModelMatrix;

uniform float ParticleRadius;

void main()
{
    mat4 VM = BBViewMatrix * BBModelMatrix;
    v2g_view_space_particle_radius = length(VM * vec4(ParticleRadius, 0.0, 0.0, 0.0));

    gl_Position = VM * BBPosition;
}