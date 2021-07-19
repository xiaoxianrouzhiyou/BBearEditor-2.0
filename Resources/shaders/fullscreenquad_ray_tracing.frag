varying vec4 V_Position;
varying vec4 V_Texcoord;

uniform sampler2D texture0;
uniform mat4 projectionMatrix_I;
uniform mat4 viewMatrix_I;

vec4 UV2PointOnPlane(vec4 point)
{
    point = 2 * point;
    point = point - vec4(1.0);
    point = viewMatrix_I * projectionMatrix_I * point;
    point = vec4(point.xyz / point.w, 1.0);
    return point;
}

void main(void)
{
    vec4 point_near_plane = UV2PointOnPlane(vec4(V_Texcoord.xy, 0.0, 1.0));
    vec4 point_far_plane = UV2PointOnPlane(vec4(V_Texcoord.xy, 1.0, 1.0));

    vec3 ray_direction = normalize(point_far_plane.xyz - point_near_plane.xyz);

    if (ray_direction.x < 0.0)
        ray_direction.x = -ray_direction.x;
    if (ray_direction.y < 0.0)
        ray_direction.y = -ray_direction.y;
    if (ray_direction.z < 0.0)
        ray_direction.z = -ray_direction.z;

    gl_FragColor = vec4(ray_direction.xyz, 1.0);
}
