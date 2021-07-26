varying vec4 V_Color;
varying vec4 V_Texcoord;

uniform sampler2D texture0;

void main(void)
{
    vec4 final_color = V_Color;

    final_color *= texture2D(texture0, V_Texcoord.xy);

    gl_FragColor = final_color;
}
