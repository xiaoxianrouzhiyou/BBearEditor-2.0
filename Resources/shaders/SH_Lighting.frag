varying vec4 V_Texcoord;

uniform vec4 BBSphericalHarmonicLightingCoefficients[16];

void main(void)
{
    vec4 final_color = vec4(0.0);
    for (int i = 0; i < 16; i++)
    {
        final_color += BBSphericalHarmonicLightingCoefficients[i];
    }

    final_color.a = 1.0;
    gl_FragColor = vec4(BBSphericalHarmonicLightingCoefficients[0].x);
    gl_FragColor.a = 1.0;
}
