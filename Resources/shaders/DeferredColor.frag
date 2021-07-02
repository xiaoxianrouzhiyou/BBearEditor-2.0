varying vec4 V_texcoord;
uniform sampler2D texture0;

void main(void)
{
    gl_FragColor = texture2D(texture0, V_texcoord.xy);
}

