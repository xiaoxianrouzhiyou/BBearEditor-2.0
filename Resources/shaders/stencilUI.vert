attribute vec4 BBPosition;

uniform mat4 BBModelMatrix;
uniform vec4 BBCanvas;

void main()
{
    vec4 screen_pos = BBModelMatrix * BBPosition;

    // Scaling to NDC coordinates
    // Z is any value of -1 ~ 1
    gl_Position = vec4(screen_pos.x / BBCanvas.x, screen_pos.y / BBCanvas.y, 0.0, 1.0);
}
