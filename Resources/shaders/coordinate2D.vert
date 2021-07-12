attribute vec4 position;
attribute vec4 color;

varying vec4 V_Color;

uniform mat4 modelMatrix;
uniform vec4 screenParameters;

void main()
{
    V_Color = color;

    vec4 screen_pos = modelMatrix * position;

    // Scaling to NDC coordinates
    // Z is any value of -1 ~ 1
    gl_Position = vec4(screen_pos.x / screenParameters.x * 2.0, screen_pos.y / screenParameters.y * 2.0, 0.0, 1.0);
}
