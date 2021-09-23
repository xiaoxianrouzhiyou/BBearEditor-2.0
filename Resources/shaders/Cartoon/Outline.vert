attribute vec4 BBPosition;
attribute vec4 BBSmoothNormal;

uniform mat4 BBProjectionMatrix;
uniform mat4 BBViewMatrix;
uniform mat4 BBModelMatrix;
uniform vec4 BBCameraPosition;

uniform float OutlineWidth;


void main()
{
    // When it is far away, the outline width will not decrease
    vec3 world_pos = (BBModelMatrix * BBPosition).xyz;
    float d = length(BBCameraPosition.xyz - world_pos);

    vec3 object_pos = BBPosition.xyz;
    vec3 w = normalize(BBSmoothNormal) * OutlineWidth * 0.0001;
    w *= d;
    object_pos += w;
    gl_Position = BBProjectionMatrix * BBViewMatrix * BBModelMatrix * vec4(object_pos, 1.0);
}
