#version 330 core

out vec4 fColor;
float near = 0.1;
float far = 100.0;

void main()
{
    float nlDepth = gl_FragCoord.z;

    /* find z-value from non linear depth */
    float ndc = 2.0 * nlDepth - 1.0;
    float zValue = (2.0 * near * far) / (far + near - ndc * (far-near));

    /* find linear depth */
    float lDepth = (zValue - near) / (far - near);

    /* near: darker(0), far: lighter(1) */
    fColor = vec4(vec3(lDepth), 1.0);
}
