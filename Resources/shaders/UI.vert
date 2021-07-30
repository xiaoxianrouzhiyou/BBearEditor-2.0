attribute vec4 BBPosition;
attribute vec4 BBTexcoord;
attribute vec4 BBColor;

varying vec4 V_Color;
varying vec4 V_Texcoord;
varying vec4 V_Light;
varying vec4 V_Dark;

uniform mat4 BBModelMatrix;
uniform vec4 BBCanvas;

void main()
{
    V_Color = BBColor;
    V_Texcoord = BBTexcoord;
    V_Light = vec4(1.0); // tangent
    V_Dark = vec4(1.0); // texcoord1

    vec4 screen_pos = BBModelMatrix * BBPosition;

    // Scaling to NDC coordinates
    // Z is any value of -1 ~ 1
    gl_Position = vec4(screen_pos.x / BBCanvas.x, screen_pos.y / BBCanvas.y, 0.0, 1.0);
}
