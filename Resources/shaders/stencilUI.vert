attribute vec4 position;

uniform mat4 modelMatrix;
uniform vec4 canvas;

void main()
{
    vec4 screen_pos = modelMatrix * position;

    // Scaling to NDC coordinates
    // Z is any value of -1 ~ 1
    gl_Position = vec4(screen_pos.x / canvas.x, screen_pos.y / canvas.y, 0.0, 1.0);
}
