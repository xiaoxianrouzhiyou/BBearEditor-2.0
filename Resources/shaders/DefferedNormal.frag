varying vec4 V_Normal;

void main(void)
{
    vec3 normal_encoded = normalize(V_Normal.xyz);
    normal_encoded = 0.5 * (normal_encoded + vec3(1.0));
    gl_FragColor = vec4(normal_encoded, 1.0);
}

