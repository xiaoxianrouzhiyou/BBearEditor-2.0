varying vec4 V_Texcoord;

uniform sampler2D texture0;

void main(void)
{
    vec4 data = texture2D(texture0, V_Texcoord.xy);
    gl_FragColor = vec4(data.xyz / data.w, 1.0);
}
