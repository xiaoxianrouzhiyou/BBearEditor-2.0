varying vec4 V_Color;
varying vec4 V_Texcoord;

uniform sampler2D texture0;

void main(void)
{
    gl_FragColor = V_Color * texture2D(texture0, V_Texcoord.xy);
}
