attribute vec4 BBPosition;
attribute vec4 BBTexcoord;

varying vec4 V_texcoord;

void main()
{
    V_texcoord = BBTexcoord;
    gl_Position = BBPosition;
}
