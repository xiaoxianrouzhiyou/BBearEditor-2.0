#version 430

varying vec4 v2f_color;
varying vec2 v2f_texcoord;

void main(void)
{
    float d = length(v2f_texcoord * 2.0 - 1.0) * 2.5;
    float a = exp(-d * d);
    if (a < 0.01)
        discard;

    gl_FragColor = vec4(v2f_color.rgb, a);
}
