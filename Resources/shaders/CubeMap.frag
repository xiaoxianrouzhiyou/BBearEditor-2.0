varying vec4 V_world_pos;
varying vec4 V_Position;
varying vec4 V_Normal;

uniform vec4 BBCameraPosition;
uniform samplerCube Cube;

void main(void)
{
    // V view, N normal, R reflect
    vec3 V = normalize(BBCameraPosition.xyz - V_world_pos.xyz);
    vec3 N = normalize(V_Normal);
    vec3 R = reflect(-V, N);
    R.y = -R.y;

    gl_FragColor = textureCube(Cube, R);
}
