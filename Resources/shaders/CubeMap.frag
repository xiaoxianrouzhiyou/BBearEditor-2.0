varying vec4 V_Texcoord;

uniform samplerCube Cube;

void main(void)
{
    gl_FragColor = textureCube(Cube, V_Texcoord);
}
