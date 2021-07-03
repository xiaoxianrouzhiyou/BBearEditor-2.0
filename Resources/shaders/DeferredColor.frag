varying vec4 V_Color;
varying vec4 V_texcoord;
uniform sampler2D texture0;
uniform vec4 textureSettings;

void main(void)
{
    vec4 color = V_Color;
    if (textureSettings.x != 0)
    {
        // there is a texture
        color = texture2D(texture0, V_texcoord.xy);
    }
    gl_FragColor = color;
}

