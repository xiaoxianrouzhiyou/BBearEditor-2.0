varying vec4 V_Color;
uniform vec4 Color_Base_Color;

void main(void)
{
    gl_FragColor = V_Color * Color_Base_Color;
}

