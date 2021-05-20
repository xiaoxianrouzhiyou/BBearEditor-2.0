varying vec4 colorVary;
varying vec4 normalVary;
varying vec4 worldPositionVary;

//摄像机的位置
uniform vec4 cameraPositionUniform;

void main(void)
{
    vec4 color = vec4(0.0, 0.0, 0.0, 0.0);
    vec3 L = normalize(cameraPositionUniform.xyz - worldPositionVary.xyz);
    vec3 N = normalize(normalVary.xyz);
    float diffuseIntensity = max(0.0, dot(L, N));
    if (diffuseIntensity > 0.0)
    {
        color = colorVary;
    }
    else
    {
        color = vec4(colorVary.xyz, 0.2);
    }
    gl_FragColor = color;
}
