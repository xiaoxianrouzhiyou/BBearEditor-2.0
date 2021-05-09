varying vec4 colorVary;
varying vec2 texcoordVary;

uniform sampler2D textureUniform;
uniform sampler2D textureUniform2;

void main(void)
{
    gl_FragColor = colorVary * texture2D(textureUniform, texcoordVary) * texture2D(textureUniform2, texcoordVary);
}
