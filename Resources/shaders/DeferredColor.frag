varying vec4 V_Color;
varying vec4 V_texcoord;
uniform sampler2D BBTexture0;
uniform vec4 BBTextureSettings;

void main(void)
{
    vec4 color = V_Color;
    if (BBTextureSettings.x != 0)
    {
        // there is a texture
        color = texture2D(BBTexture0, V_texcoord.xy);
    }
    gl_FragColor = color;
}

