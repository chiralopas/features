#version 330 core

layout (location = 0) in vec4 vPosition;
uniform mat4 uModel, uView, uProjection;
uniform vec4 uClipPlane;

void main()
{
    /* distance from this vertex to the clip plane */
    gl_ClipDistance[0] = dot(vPosition, uClipPlane);

    gl_Position = uProjection * uView * uModel * vPosition;
}