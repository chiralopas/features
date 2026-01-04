#version 330 core

in vec2 texCoords;
out vec4 fColor;

/* sampling from texture unit 0 (default) */
uniform sampler2D screenTexture;

void main()
{
    fColor = texture(screenTexture, texCoords);

    /* grayscale effect */
    float average = (fColor.r + fColor.g + fColor.b) / 3.0f;
    fColor = vec4(average, average, average, 1.0);
}