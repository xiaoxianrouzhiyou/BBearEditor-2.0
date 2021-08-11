varying vec3 V_normal;

uniform vec4 BBSphericalHarmonicLightingCoefficients[16];

const float PI = 3.1415926535897932384626433832795;

void main(void)
{
    vec4 final_color = vec4(0.0);

    if ((abs(V_normal.x) < 0.0001) && (abs(V_normal.y) < 0.0001) && (abs(V_normal.z) < 0.0001))
    {
        final_color = vec4(0.0, 0.0, 0.0, 1.0);
    }
    else
    {
        float x = V_normal.x;
        float y = V_normal.y;
        float z = V_normal.z;
        float x2 = x * x;
        float y2 = y * y;
        float z2 = z * z;

        float basis[9];
        basis[0] = 1.0 / 2.0 * sqrt(1.0 / PI);
        basis[1] = 2.0 / 3.0 * sqrt(3.0 / (4.0 * PI)) * z;
        basis[2] = 2.0 / 3.0 * sqrt(3.0 / (4.0 * PI)) * y;
        basis[3] = 2.0 / 3.0 * sqrt(3.0 / (4.0 * PI)) * x;
        basis[4] = 1.0 / 4.0 * 1.0 / 2.0 * sqrt(15.0 / PI) * x * z;
        basis[5] = 1.0 / 4.0 * 1.0 / 2.0 * sqrt(15.0 / PI) * z * y;
        basis[6] = 1.0 / 4.0 * 1.0 / 4.0 * sqrt(5.0 / PI) * (-x2 - z2 + 2 * y2);
        basis[7] = 1.0 / 4.0 * 1.0 / 2.0 * sqrt(15.0 / PI) * y * x;
        basis[8] = 1.0 / 4.0 * 1.0 / 4.0 * sqrt(15.0 / PI) * (x2 - z2);

        vec3 diffuse = vec3(0.0);
        for (int i = 0; i < 9; i++)
        {
            diffuse += BBSphericalHarmonicLightingCoefficients[i] * basis[i];
        }
        final_color = vec4(diffuse, 1.0);
    }

    gl_FragColor = final_color;
}
