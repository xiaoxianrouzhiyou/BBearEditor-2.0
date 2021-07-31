attribute vec4 BBPosition;
attribute vec4 BBTexcoord;

varying vec4 V_Texcoord;

uniform mat4 BBProjectionMatrix;
uniform mat4 BBViewMatrix;
uniform mat4 BBModelMatrix;

void main()
{
    V_Texcoord = BBTexcoord;
    // vec3 world_normal = mat3(transpose(inverse(BBModelMatrix))) * BBNormal.xyz;
    // vec3 world_tangent;
    // vec3 world_binormal = cross();
    // vec3 tangent_space0 = vec3(world_tangent.x, world_binormal.x, V_World_normal.x);
    // vec3 tangent_space1 = vec3(world_tangent.y, world_binormal.y, V_World_normal.y);
    // vec3 tangent_space2 = vec3(world_tangent.z, world_binormal.z, V_World_normal.z);

    gl_Position = BBProjectionMatrix * BBViewMatrix * BBModelMatrix * BBPosition;
}
