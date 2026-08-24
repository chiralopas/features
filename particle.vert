#version 330 core

layout (location = 0) in vec4 vPosition;
uniform float uTime;

void main()
{
    float twinkle = sin(uTime*2.5 + float(gl_VertexID))*0.5 + 0.5;
    gl_PointSize = 10.0 + twinkle * 20.0;
    gl_Position = vPosition;
}
