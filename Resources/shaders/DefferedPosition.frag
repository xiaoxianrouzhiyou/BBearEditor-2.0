varying vec4 V_WorldPos;

void main(void)
{
//    float scale = 1.0;
//    float w = 1.0 / scale;
//    gl_FragColor = vec4(V_WorldPos.x / scale, V_WorldPos.y / scale, V_WorldPos.z / scale, w);
    gl_FragColor = V_WorldPos;
}

