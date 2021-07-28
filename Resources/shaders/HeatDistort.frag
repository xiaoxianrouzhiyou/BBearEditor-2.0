varying vec4 V_Texcoord;

uniform sampler2D ColorFBO;

void main(void)
{
    gl_FragColor = texture2D(ColorFBO, V_Texcoord.xy);
}
