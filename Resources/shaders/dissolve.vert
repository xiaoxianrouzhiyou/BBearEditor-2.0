#version 450 core

in vec4 position;
in vec4 texcoord;

out vec4 V_Texcoord;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

void main()
{
    V_Texcoord = texcoord;

    gl_Position = projectionMatrix * viewMatrix * modelMatrix * position;
}
