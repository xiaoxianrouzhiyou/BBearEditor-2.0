varying vec4 V_Texcoord;

uniform sampler2D texture0;

void main(void)
{
    gl_FragColor = texture2D(texture0, V_Texcoord);
}
