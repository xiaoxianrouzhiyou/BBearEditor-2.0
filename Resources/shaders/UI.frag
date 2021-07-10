varying vec4 V_Color;
varying vec4 V_Texcoord;
varying vec4 V_Light;
varying vec4 V_Dark;

uniform vec4 textureSettings;
uniform sampler2D texture0;

void main(void)
{
    vec4 final_color = V_Color;
    if (textureSettings.x == 1.0)
    {
        final_color *= texture2D(texture0, V_Texcoord);
        float alpha = final_color.a * V_Light.a;
        final_color.a = alpha;
        final_color.rgb = ((vec3(alpha - 1.0) * V_Dark.a + 1.0) - final_color.rgb) * V_Dark.rgb + final_color.rgb * V_Light.rgb;
    }
    gl_FragColor = final_color;
}
