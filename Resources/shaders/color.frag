varying vec4 V_Color;
uniform vec4 BBColor_Base_Color;

void main(void)
{
    gl_FragColor = V_Color * BBColor_Base_Color;
}

