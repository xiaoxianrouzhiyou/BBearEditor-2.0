attribute vec4 BBPosition;
attribute vec4 BBNormal;

varying vec3 V_normal;

uniform mat4 BBProjectionMatrix;
uniform mat4 BBViewMatrix;
uniform mat4 BBModelMatrix;

void main()
{
    V_normal = normalize(mat3(transpose(inverse(BBViewMatrix * BBModelMatrix))) * BBNormal.xyz);
    V_normal.y = -V_normal.y;
    gl_Position = BBProjectionMatrix * BBViewMatrix * BBModelMatrix * BBPosition;
}
