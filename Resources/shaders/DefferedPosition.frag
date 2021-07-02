varying vec4 V_WorldPos;

void main(void)
{
    vec3 world_pos_scaled = V_WorldPos.xyz / 1.0;
    vec3 world_pos_encoded = 0.5 * (world_pos_scaled + vec3(1.0));
    gl_FragColor = vec4(world_pos_encoded, 1.0);
}

