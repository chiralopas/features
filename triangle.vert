#version 330 core

layout (location = 0) in vec2 vPosition;
uniform vec2 offsets[4];

void main()
{
    vec2 offset = offsets[gl_InstanceID];
    gl_Position = vec4(vPosition + offset, 0.0, 1.0);
}
