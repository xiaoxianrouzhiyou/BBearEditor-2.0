varying vec4 V_texcoord;

uniform sampler2D BBTexture0;

void main(void)
{
    gl_FragColor = texture2D(BBTexture0, V_texcoord);
}
