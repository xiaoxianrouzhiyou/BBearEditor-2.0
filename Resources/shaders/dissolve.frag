#version 450 core

in vec4 V_Texcoord;

uniform sampler2D Texture0;
uniform sampler2D Texture1;
uniform sampler2D DissolveTex_R;
uniform sampler2D RampTex_RGB;

uniform float clip;

out vec4 frag_color;

void main(void)
{
    vec4 dissolve_color = texture(DissolveTex_R, V_Texcoord.xy);
    vec2 dissolve_value = clamp((dissolve_color.rr - clip) / 0.1, 0.0, 1.0);
    vec4 ramp_color = texture(RampTex_RGB, dissolve_value);
    if (dissolve_color.r < clip)
    {
        discard;
    }

    frag_color = texture(Texture0, V_Texcoord.xy) + texture(Texture1, V_Texcoord.xy) + ramp_color;
}
