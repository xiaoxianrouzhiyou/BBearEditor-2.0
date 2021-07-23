#version 450 core

in vec4 V_Texcoord;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D dissolve;

uniform float clip;

out vec4 fragColor;

void main(void)
{
    vec4 dissolve_tex = texture(dissolve, V_Texcoord.xy);
    if (dissolve_tex.r < clip)
    {
        discard;
    }

    fragColor = texture(texture0, V_Texcoord.xy) * texture(texture1, V_Texcoord.xy);
}
