varying vec4 V_Color;
varying vec4 V_Texcoord;
varying vec4 V_Light;
varying vec4 V_Dark;

uniform vec4 BBTextureSettings;
uniform sampler2D BBTexture0;

void main(void)
{
    gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}
