varying vec4 colorVary;
varying vec4 normalVary;
varying vec4 worldPositionVary;

uniform vec4 lightPositionUniform;
uniform vec4 lightAmbientUniform;
uniform vec4 ambientMaterialUniform;
uniform vec4 lightDiffuseUniform;
uniform vec4 diffuseMaterialUniform;

void main(void)
{
    vec4 color = vec4(0.0, 0.0, 0.0, 0.0);
    float distance = 0.0;
    float constantFactor = 0.5;
    float linearFactor = 0.3;
    float quadricFactor = 0.1;

    vec4 ambientColor = lightAmbientUniform * ambientMaterialUniform;

    vec3 L = lightPositionUniform.xyz - worldPositionVary.xyz;
    distance = length(L);
    float attenuation = 1.0 / (constantFactor + linearFactor * distance + quadricFactor * quadricFactor * distance);
    L = normalize(L);

    vec3 N = normalize(normalVary.xyz);
    float diffuseIntensity = max(0.0, dot(L, N));
    vec4 diffuseColor = lightDiffuseUniform * diffuseMaterialUniform * diffuseIntensity * attenuation;

    color = ambientColor + diffuseColor;
    gl_FragColor = color * colorVary;
}
