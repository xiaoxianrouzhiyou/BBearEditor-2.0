varying vec4 V_Texcoord;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main(void)
{
    vec4 data = texture2D(texture1, V_Texcoord.xy);
    gl_FragColor = vec4(data.xyz, 1.0);
}
