varying vec4 V_Texcoord;

uniform sampler2D Albedo;
uniform vec4 BBColor_Color;

void main(void)
{
    gl_FragColor = BBColor_Color * texture2D(Albedo, V_Texcoord.xy);
}
