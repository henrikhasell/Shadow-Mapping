#version 130

out float out_Color;

void main(void)
{
    out_Color = gl_FragCoord.z;
}