varying vec4 V_Texcoord;

uniform sampler2D BBShadowMap;

void main(void)
{
    gl_FragColor = texture2D(BBShadowMap, V_Texcoord);
}
