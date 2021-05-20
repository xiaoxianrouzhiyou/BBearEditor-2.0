varying vec4 colorVary;
varying vec2 texcoordVary;

uniform sampler2D textureUniform;

void main(void)
{
    gl_FragColor = colorVary * texture2D(textureUniform, texcoordVary);
}
