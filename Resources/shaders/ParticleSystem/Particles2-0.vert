attribute vec4 BBPosition;

uniform mat4 BBModelMatrix;

void main()
{
    gl_Position = BBModelMatrix * BBPosition;
}
