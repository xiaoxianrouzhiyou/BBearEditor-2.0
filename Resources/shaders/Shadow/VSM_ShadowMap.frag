#version 430 core

layout (location = 0) out vec2 FragColor;

void main(void)
{
    float depth = gl_FragCoord.z;
	FragColor = vec2(depth, depth * depth);
}
