#version 330 core

layout (location = 0) out vec4 fColor;

in vec4 oColor;

void main()
{
    fColor = oColor;
}
