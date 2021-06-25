varying vec4 texcoordVary;

uniform sampler2D textureUniform;

void main(void)
{
    vec4 data = texture2D(textureUniform, texcoordVary.xy);
    gl_FragColor = vec4(data.xyz / data.w, 1.0);
}
