attribute vec4 positionAttr;
attribute vec4 colorAttr;
attribute vec2 texcoordAttr;
attribute vec4 normalAttr;

varying vec4 colorVary;

uniform sampler2D textureUniform;

uniform vec4 lightAmbientUniform;
uniform vec4 ambientMaterialUniform;
uniform vec4 lightDiffuseUniform;
uniform vec4 diffuseMaterialUniform;
uniform vec4 lightSpecularUniform;
uniform vec4 specularMaterialUniform;
//光源的位置
uniform vec4 directionLightPositionUniform1;
//摄像机的位置 视点不同 镜面反光点不同
uniform vec4 cameraPositionUniform;
uniform vec4 lightOptionUniform;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
//正确计算法线
uniform mat4 IT_modelMatrix;


void main()
{
    vec4 color = vec4(0.0, 0.0, 0.0, 0.0);
    //环境光分量*材质环境光反射系数
    vec4 ambientColor = lightAmbientUniform * ambientMaterialUniform;

    vec3 L = normalize(directionLightPositionUniform1.xyz);
    vec4 normal = IT_modelMatrix * normalAttr;
    vec3 N = normalize(normal.xyz);
    float diffuseIntensity = max(0.0, dot(L, N));
    vec4 diffuseColor = lightDiffuseUniform * diffuseMaterialUniform * diffuseIntensity;

    vec4 specularColor = vec4(0.0, 0.0, 0.0, 0.0);
    //漫反射强度不为0才有可能具有镜面反射
    if (diffuseIntensity != 0.0)
    {
        //比计算反射光线简化计算量 加法代替reflect函数
        vec4 worldPosition = modelMatrix * positionAttr;
        vec3 viewDir = normalize(cameraPositionUniform.xyz - worldPosition.xyz);
        vec3 halfVector = normalize(L + viewDir);
        specularColor = lightSpecularUniform * specularMaterialUniform * pow(max(0.0, dot(N, halfVector)), lightOptionUniform.x);
    }

    //color = ambientColor + diffuseColor + specularColor;
    if (lightOptionUniform.w == 1.0)
    {
        color = ambientColor + diffuseColor * texture2D(textureUniform, texcoordAttr) + specularColor;
    }
    else
    {
        color = (ambientColor + diffuseColor) * texture2D(textureUniform, texcoordAttr);
    }

    colorVary = color;
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * positionAttr;
}

