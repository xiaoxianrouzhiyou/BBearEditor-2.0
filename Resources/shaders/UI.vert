attribute vec4 position;
attribute vec4 texcoord;
attribute vec4 color;

varying vec4 V_Color;
varying vec4 V_Texcoord;
varying vec4 V_Light;
varying vec4 V_Dark;

uniform mat4 modelMatrix;
uniform vec4 canvas;

void main()
{
    V_Color = color;
    V_Texcoord = texcoord;
    V_Light = vec4(1.0); // tangent
    V_Dark = vec4(1.0); // texcoord1

    vec4 screen_pos = modelMatrix * position;

    // Scaling to NDC coordinates
    // Z is any value of -1 ~ 1
    gl_Position = vec4(screen_pos.x / canvas.x, screen_pos.y / canvas.y, 0.0, 1.0);
}
