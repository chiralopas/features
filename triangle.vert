#version 330 core

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec2 vTexCoord;

out vec2 texCoord;

uniform mat4 uModel, uView, uProjection;

void main()
{
    gl_Position = uProjection * uView * uModel * vPosition;
    texCoord = vTexCoord;
}
