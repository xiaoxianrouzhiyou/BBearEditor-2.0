#version 120
varying vec4 colorVary;
varying vec2 texcoordVary;
varying vec4 normalVary;
varying vec4 worldPositionVary;
varying vec4 viewSpacePositionVary;

uniform sampler2D textureUniform;

uniform vec4 lightAmbientUniform;
uniform vec4 ambientMaterialUniform;
uniform vec4 diffuseMaterialUniform;
uniform vec4 lightSpecularUniform;
uniform vec4 specularMaterialUniform;
//光源的位置
uniform vec4 lightCountUniform;
uniform vec4 directionLightPositionUniform[8];
uniform vec4 directionLightColorUniform[8];
uniform vec4 pointLightPositionUniform[64];
uniform vec4 pointLightColorUniform[64];
uniform vec4 pointLightOptionUniform[64];
uniform vec4 spotLightPositionUniform[64];
uniform vec4 spotLightDirectionUniform[64];
uniform vec4 spotLightColorUniform[64];
uniform vec4 spotLightOptionUniform[64];
uniform vec4 spotLightOption2Uniform[64];
//摄像机的位置 视点不同 镜面反光点不同
uniform vec4 cameraPositionUniform;
uniform vec4 lightOptionUniform;

uniform bool fogSwitchUniform;
uniform vec4 fogColorUniform;
uniform vec4 fogOptionUniform;
uniform vec4 fogModeUniform;

//阴影
uniform sampler2D shadowMapUniform;
varying vec4 lightSpacePosVary;

//雾
float calculateFog(float distance)
{
    //离得越远 雾越浓 alpha越大
    float fogAlpha;
    if (fogModeUniform.x == 0.0)
    {
        //用线性的方式计算alpha
        fogAlpha = (distance - fogOptionUniform.x) / (fogOptionUniform.y - fogOptionUniform.x);
        fogAlpha = 1.0 - clamp(fogAlpha, 0.0, 1.0);
    }
    else if (fogModeUniform.x == 1.0)
    {
        fogAlpha = exp(-(distance * fogOptionUniform.z / 500));
        fogAlpha = clamp(fogAlpha, 0.0, 1.0);
    }
    else
    {
        fogAlpha = exp(-pow(distance * fogOptionUniform.z / 500, fogOptionUniform.w));
        fogAlpha = clamp(fogAlpha, 0.0, 1.0);
    }
    return fogAlpha;
}

//计算当前像素点是否有阴影
float calculateShadow()
{
    vec3 pos = lightSpacePosVary.xyz / lightSpacePosVary.w;
    //-1~1 化为0~1 纹理坐标范围
    //目前在-1~0
    vec2 posUV = pos.xy + vec2(1.0);
    //当前渲染的像素在灯光视口下的深度图中 取出对应的深度信息
    //float depthInShadowMap = texture2D(shadowMapUniform, posUV).r;
    //当前像素实际深度值
    float currentDepth = pos.z * 0.5 + 0.5;
    currentDepth = clamp(currentDepth, 0.0, 1.0);
    //向旁边取像素的偏移值
    vec2 texelSize = vec2(1.0 / 500.0, 1.0 / 500.0);
    float shadow = 0.0;
    for (int y = -1; y <= 1; y++)
    {
        for (int x = -1; x <= 1; x++)
        {
            float depthInShadowMap = texture2D(shadowMapUniform, posUV + texelSize * vec2(x, y)).r;
            shadow += (currentDepth - 0.004) > depthInShadowMap ? 1.0 : 0.0;
        }
    }
    //实际深度值 与深度图上的值比较
    //float shadow = (currentDepth - 0.005) > depthInShadowMap ? 1.0 : 0.0;
    shadow /= 9.0;
    return shadow;
}

void main(void)
{
    vec3 N = normalize(normalVary.xyz);
    vec3 viewDir = normalize(cameraPositionUniform.xyz - worldPositionVary.xyz);
    //环境光分量*材质环境光反射系数
    vec4 ambientColor = lightAmbientUniform * ambientMaterialUniform;
    //平行光个数
    int count = int(lightCountUniform.x);
    vec3 L = vec3(0.0, 0.0, 0.0);
    vec4 diffuseColor = vec4(0.0, 0.0, 0.0, 0.0);
    float specularIntensity = 0.0;
    for (int i = 0; i < count; i++)
    {
        L = normalize(directionLightPositionUniform[i].xyz);
        float diffuseIntensity = max(0.0, dot(L, N));
        diffuseColor += directionLightColorUniform[i] * diffuseIntensity;
        //漫反射强度不为0才有可能具有镜面反射
        if (diffuseIntensity != 0.0)
        {
            vec3 reflectDir = normalize(reflect(-L, N));
            specularIntensity += pow(max(0.0, dot(reflectDir, viewDir)), lightOptionUniform.x);
        }
    }
    diffuseColor = diffuseColor * diffuseMaterialUniform;
    vec4 specularColor = lightSpecularUniform * specularMaterialUniform * specularIntensity;
    //点光源
    count = int(lightCountUniform.y);
    vec4 color = vec4(0.0, 0.0, 0.0, 0.0);
    for (int i = 0; i < count; i++)
    {
        float constantFactor = pointLightOptionUniform[i].x;
        float linearFactor = pointLightOptionUniform[i].y;
        float quadricFactor = pointLightOptionUniform[i].z;
        L = pointLightPositionUniform[i].xyz - worldPositionVary.xyz;
        float distance = length(L);
        float attenuation = 1.0 / (constantFactor + linearFactor * distance + quadricFactor * quadricFactor * distance);
        L = normalize(L);
        float diffuseIntensity = max(0.0, dot(L, N));
        color += pointLightColorUniform[i] * diffuseIntensity * attenuation * pointLightOptionUniform[i].w;
    }
    color *= diffuseMaterialUniform;
    diffuseColor += color;
    //聚光灯
    count = int(lightCountUniform.z);
    color = vec4(0.0, 0.0, 0.0, 0.0);
    for (int i = 0; i < count; i++)
    {
        float constantFactor = spotLightOptionUniform[i].x;
        float linearFactor = spotLightOptionUniform[i].y;
        float quadricFactor = spotLightOptionUniform[i].z;
        L = spotLightPositionUniform[i].xyz - worldPositionVary.xyz;
        float distance = length(L);
        float attenuation = 1.0 / (constantFactor + linearFactor * distance + quadricFactor * quadricFactor * distance);
        L = normalize(L);
        float diffuseIntensity = max(0.0, dot(L, N));
        if (diffuseIntensity > 0.0)
        {
            vec3 spotDirection = normalize(spotLightDirectionUniform[i].xyz);
            //当前点与聚光灯方向 夹角的余弦值
            float currentCos = max(0.0, dot(-L, spotDirection));
            //cutoff余弦值
            float radianCutoff = spotLightOptionUniform[i].w / 2 * 3.14 / 180.0;
            float cutoffCos = cos(radianCutoff);
            if (currentCos > cutoffCos)
            {
                //在cutoff范围内
                diffuseIntensity = pow(currentCos, spotLightOption2Uniform[i].x) * spotLightOption2Uniform[i].y;
            }
            else
            {
                diffuseIntensity = 0.0;
            }
        }
        color += spotLightColorUniform[i] * diffuseIntensity * attenuation;
    }
    color *= diffuseMaterialUniform;
    diffuseColor += color;
    //最终
    //color = ambientColor + diffuseColor + specularColor;
    if (lightOptionUniform.w == 1.0)
    {
        color = ambientColor + diffuseColor * texture2D(textureUniform, texcoordVary) + specularColor;
    }
    else
    {
        color = (ambientColor + diffuseColor) * texture2D(textureUniform, texcoordVary);
    }
    //雾的颜色和物体本身颜色进行混合
    if (fogSwitchUniform)
    {
        float fogAlpha = calculateFog(abs(viewSpacePositionVary.z / viewSpacePositionVary.w));
        color = mix(fogColorUniform, color, fogAlpha);
    }

    //如果点在阴影区 给上阴影颜色
    color = color * vec4(vec3(1.0 - calculateShadow()), 1.0);

    gl_FragColor = color;
}
