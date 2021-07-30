varying vec4 V_Texcoord;
varying vec4 V_ScreenUV;

uniform sampler2D BBColorFBO;
uniform sampler2D DistortTex;
uniform float Distort;

void main(void)
{
    vec4 distort_color = texture2D(DistortTex, V_Texcoord.xy);
    vec2 uv = mix(V_ScreenUV.xy, distort_color.xy, Distort);
    gl_FragColor = texture2D(BBColorFBO, uv);
}
