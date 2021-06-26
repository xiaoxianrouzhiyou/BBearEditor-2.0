varying vec4 V_Normal;

void main(void)
{
    gl_FragColor = vec4(normalize(V_Normal.xyz), 1.0);
}

