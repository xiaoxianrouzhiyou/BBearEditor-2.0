attribute vec4 positionAttr;
attribute vec4 texcoordAttr;

varying vec4 texcoordVary;

void main()
{
    texcoordVary = texcoordAttr;
    gl_Position = positionAttr;
}
