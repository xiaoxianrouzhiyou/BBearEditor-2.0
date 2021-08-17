#version 120

varying vec4 V_color;

uniform sampler2D BBTexture0;

void main(void)
{
    gl_FragColor = texture2D(BBTexture0, gl_PointCoord.xy) * V_color;
}
