attribute vec4 BBPosition;
attribute vec4 BBColor;

varying vec4 V_Color;

uniform mat4 BBModelMatrix;
uniform vec4 BBScreenParameters;

void main()
{
    V_Color = BBColor;

    vec4 screen_pos = BBModelMatrix * BBPosition;

    // Scaling to NDC coordinates
    // Z is any value of -1 ~ 1
    gl_Position = vec4(screen_pos.x / BBScreenParameters.x * 2.0, screen_pos.y / BBScreenParameters.y * 2.0, 0.0, 1.0);
}
