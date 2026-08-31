#version 330 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;

out vec3 fragPos;
out vec3 fragNormal;

uniform mat4 uModel, uView, uProjection;
uniform mat4 uNormalMat;

void main()
{
    /* world-space position and normal */
    fragPos = vec3(uModel * vec4(vPosition, 1.0));
    fragNormal = mat3(uNormalMat) * vNormal;

    gl_Position = uProjection * uView * uModel * vec4(vPosition, 1.0);
}
