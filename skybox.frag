#version 330 core

out vec4 fColor;
in vec3 texCoord;

uniform samplerCube uTexture;

void main()
{
    fColor = texture(uTexture, texCoord);
}
