#version 330 core

layout (location = 0) in vec3 vPosition;
out vec3 texCoord;

uniform mat4 uView, uProjection;

void main()
{
    /* vertex position is sample direction */
    texCoord = vPosition;
    
    /* strip translation to make skybox unreachable */
    mat4 view_rot = mat4(mat3(uView));
    vec4 pos = uProjection * view_rot * vec4(vPosition, 1.0);
    
    /* set z to w so depth is always 1.0 (furthest) */
    gl_Position = pos.xyww;
}
