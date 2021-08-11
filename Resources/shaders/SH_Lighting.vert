attribute vec4 BBPosition;
attribute vec4 BBNormal;

varying vec3 V_normal;

uniform mat4 BBProjectionMatrix;
uniform mat4 BBViewMatrix;
uniform mat4 BBModelMatrix;

void main()
{
    V_normal = normalize(mat3(transpose(inverse(BBViewMatrix * BBModelMatrix))) * BBNormal.xyz);
    gl_Position = BBProjectionMatrix * BBViewMatrix * BBModelMatrix * BBPosition;
}
