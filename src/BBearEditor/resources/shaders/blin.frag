varying vec4 colorVary;
varying vec2 texcoordVary;
varying vec4 normalVary;
varying vec4 worldPositionVary;

uniform sampler2D textureUniform;

uniform vec4 lightAmbientUniform;
uniform vec4 ambientMaterialUniform;
uniform vec4 lightDiffuseUniform;
uniform vec4 diffuseMaterialUniform;
uniform vec4 lightSpecularUniform;
uniform vec4 specularMaterialUniform;
//光源的位置
uniform vec4 lightPositionUniform;
//摄像机的位置 视点不同 镜面反光点不同
uniform vec4 cameraPositionUniform;
uniform vec4 lightOptionUniform;

void main(void)
{
    vec4 color = vec4(0.0, 0.0, 0.0, 0.0);
    //环境光分量*材质环境光反射系数
    vec4 ambientColor = lightAmbientUniform * ambientMaterialUniform;

    vec3 L = normalize(lightPositionUniform.xyz);
    vec3 N = normalize(normalVary.xyz);
    float diffuseIntensity = max(0.0, dot(L, N));
    vec4 diffuseColor = lightDiffuseUniform * diffuseMaterialUniform * diffuseIntensity;

    vec4 specularColor = vec4(0.0, 0.0, 0.0, 0.0);
    //漫反射强度不为0才有可能具有镜面反射
    if (diffuseIntensity != 0.0)
    {
        //比计算反射光线简化计算量 加法代替reflect函数
        vec3 viewDir = normalize(cameraPositionUniform.xyz - worldPositionVary.xyz);
        vec3 halfVector = normalize(L + viewDir);
        specularColor = lightSpecularUniform * specularMaterialUniform * pow(max(0.0, dot(N, halfVector)), lightOptionUniform.x);
    }

    //color = ambientColor + diffuseColor + specularColor;
    if (lightOptionUniform.w == 1.0)
    {
        color = ambientColor + diffuseColor * texture2D(textureUniform, texcoordVary) + specularColor;
    }
    else
    {
        color = (ambientColor + diffuseColor) * texture2D(textureUniform, texcoordVary);
    }
    gl_FragColor = color;
}
