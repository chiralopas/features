#version 330 core

out vec2 vNDC;

void main()
{
    /* full screen triangle */
    float x = (gl_VertexID & 1) * 4 - 1;
    float y = (gl_VertexID & 2) * 2 - 1;
    vNDC = vec2(x, y);
    gl_Position = vec4(vNDC, 0.0, 1.0);
}
