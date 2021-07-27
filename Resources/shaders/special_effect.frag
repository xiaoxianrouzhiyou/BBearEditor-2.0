varying vec4 V_Color;
varying vec4 V_Texcoord;

uniform sampler2D Texture0;
uniform vec4 Color_Base_Color;
uniform vec4 time;

void main(void)
{
    vec4 final_color = V_Color;

    final_color *= texture2D(Texture0, V_Texcoord.xy + vec2(0.0001, 0.0001) * time.z);
    final_color *= Color_Base_Color;

    gl_FragColor = final_color;
}
