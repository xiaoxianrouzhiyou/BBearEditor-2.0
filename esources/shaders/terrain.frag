varying vec4 colorVary;
varying vec2 texcoordVary;
varying vec4 normalVary;

uniform sampler2D textureUniform;

uniform vec4 lightAmbientUniform;
uniform vec4 ambientMaterialUniform;
uniform vec4 lightDiffuseUniform;
uniform vec4 diffuseMaterialUniform;
uniform vec4 lightPositionUniform;

void main(void)
{
    vec3 N = normalize(normalVary.xyz);
    vec4 ambientColor = lightAmbientUniform * ambientMaterialUniform;
    vec3 L = normalize(lightPositionUniform.xyz);
    float diffuseIntensity = max(0.0, dot(L, N));
    vec4 diffuseColor = diffuseMaterialUniform * lightDiffuseUniform * diffuseIntensity;

    gl_FragColor = (ambientColor + diffuseColor) * texture2D(textureUniform, texcoordVary);
}
