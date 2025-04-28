#version 330 core

layout (location = 0) in vec4 vPosition;

uniform Transformation {
  vec3 translation;
  vec4 rotation;
  float scale;
};

void main()
{
    vec3 pos = vPosition.xyz;
    float angle = radians(rotation[0]);
    vec3 axis = normalize(rotation.yzw);
    mat3 I = mat3(1.0);
    mat3 S = mat3(0, -axis.z, axis.y, 
                 axis.z, 0,-axis.x, 
                -axis.y, axis.x, 0);
    mat3 uuT = outerProduct(axis, axis);
    mat3 rot = uuT + cos(angle)*(I- uuT) + sin(angle)*S;
    pos *= scale;
    pos *= rot;
    pos += translation;

    gl_Position = vec4(pos, 1);
}
