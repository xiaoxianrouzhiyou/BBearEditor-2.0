varying vec4 texcoordVary;

uniform sampler2D textureUniform;

void main(void)
{
    gl_FragColor = texture2D(textureUniform, texcoordVary);
}
