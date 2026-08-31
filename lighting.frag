#version 330 core
out vec4 fragColor;

in vec3 fragPos;
in vec3 fragNormal;

uniform vec3 uLightPos;
uniform vec3 uLightColor;
uniform vec3 uViewPos;
uniform vec3 uObjectColor;

void main()
{
    // ambient
    float ambient_strength = 0.1;
    vec3 ambient = ambient_strength * uLightColor;

    // diffuse
    float diffuse_strength = 0.8;
    vec3 normal = normalize(fragNormal);
    vec3 light_dir = normalize(uLightPos - fragPos);
    float diffuse_factor = max(dot(normal, light_dir), 0.0);
    vec3 diffuse = diffuse_strength * diffuse_factor * uLightColor;

    // specular
    float specular_strength = 0.5;
    vec3 view_dir = normalize(uViewPos - fragPos);
    vec3 reflect_dir = reflect(-light_dir, normal);
    float specular_factor = pow(max(dot(view_dir, reflect_dir), 0.0), 32);
    vec3 specular = specular_strength * specular_factor * uLightColor;

    /* Phong lighting */
    vec3 result = (ambient + diffuse + specular) * uObjectColor;
    fragColor = vec4(result, 1.0);
}
