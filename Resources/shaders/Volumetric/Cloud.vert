#version 330 core

in vec4 BBPosition;
in vec4 BBTexcoord;

out vec2 v2f_texcoord;
out mat4 v2f_inverse_projection_matrix;
out mat4 v2f_inverse_view_matrix;

uniform mat4 BBProjectionMatrix;
uniform mat4 BBViewMatrix;
uniform mat4 BBModelMatrix;

void main()
{
    gl_Position = BBPosition;

    v2f_texcoord = BBTexcoord.xy;
    v2f_inverse_projection_matrix = inverse(BBProjectionMatrix);
    v2f_inverse_view_matrix = inverse(BBViewMatrix);
}
