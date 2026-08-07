#version 330 core

in vec2 texCoords;
out vec4 fColor;

uniform sampler2D uScreenColor;
uniform sampler2D uScreenDepth;

float near = 0.1, far = 100.0;
float fogStart = 3.5, fogEnd = 12.0;
vec3 fogColor = vec3(0.7, 0.75, 0.8);


float linearizeDepth(float d) 
{
    float z = d * 2.0 - 1.0;
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{
    vec3 color = texture(uScreenColor, texCoords).rgb;
    float dist = linearizeDepth(texture(uScreenDepth, texCoords).r);
    float fogFactor = clamp((fogEnd - dist) / (fogEnd - fogStart), 0.0, 1.0);
    fColor = vec4(mix(fogColor, color, fogFactor), 1.0);
}