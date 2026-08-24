#version 330 core

out vec4 fColor;
uniform sampler2D uTexture;

void main()
{
    fColor = texture(uTexture, gl_PointCoord);
}
