varying vec4 V_Position;

uniform samplerCube Cube;

void main(void)
{
    gl_FragColor = textureCube(Cube, V_Position.xyz);
}
