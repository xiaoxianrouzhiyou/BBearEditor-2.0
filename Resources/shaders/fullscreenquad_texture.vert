attribute vec4 BBPosition;
attribute vec4 BBTexcoord;

varying vec4 V_Texcoord;

void main()
{
    V_Texcoord = BBTexcoord;
    gl_Position = BBPosition;
}
