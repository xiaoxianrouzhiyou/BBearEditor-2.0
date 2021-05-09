#version 120
//版本高于120才能取到gl_PointCoord变量
uniform sampler2D textureUniform;
varying vec4 colorVary;

void main(void)
{
    gl_FragColor = colorVary * texture2D(textureUniform, gl_PointCoord.xy);
}
