varying vec4 V_local_pos;

uniform samplerCube BBSkyBox;

void main(void)
{
    gl_FragColor = textureCube(BBSkyBox, V_local_pos.xyz);
}
