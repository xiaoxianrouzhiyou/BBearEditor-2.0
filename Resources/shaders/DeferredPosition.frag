varying vec4 V_WorldPos;

void main(void)
{
//    float scale = 1.0;
//    float w = 1.0 / scale;
//    vec3 world_pos_scaled = V_WorldPos.xyz / scale;
//    vec3 world_pos_encoded = 0.5 * (world_pos_scaled + vec3(1.0));
//    gl_FragColor = vec4(world_pos_encoded, w);
    gl_FragColor = V_WorldPos;
}

