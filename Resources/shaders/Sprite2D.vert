attribute vec4 position;
attribute vec4 texcoord;

uniform mat4 modelMatrix;

varying vec4 V_Texcoord;

void main()
{
    V_Texcoord = texcoord;
    gl_Position = modelMatrix * position;
}
