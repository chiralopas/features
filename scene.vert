#version 330 core

layout (location = 0) in vec4 vPosition;
uniform mat4 uModel, uView, uProjection;

void main()
{
    gl_Position = uProjection * uView * uModel * vPosition;
}
