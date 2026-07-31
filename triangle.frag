#version 330 core

out vec4 fColor;
in vec2 texCoord;

uniform sampler2DArray uTexture;
uniform int uSlice;

void main()
{
    fColor = texture(uTexture, vec3(texCoord, uSlice));
}
