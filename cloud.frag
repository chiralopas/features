#version 330 core

in vec2 vNDC;
out vec4 fColor;

uniform sampler3D uTexture;
uniform mat4 uModel, uView, uProjection;

void main()
{
    /* Ray Setup: build a ray per pixel */
    mat4 inv_vp = inverse(uProjection * uView);
    vec4 world_far = inv_vp * vec4(vNDC, 1.0, 1.0);
    vec3 camera_pos = inverse(uView)[3].xyz;
    vec3 ray_dir = normalize(world_far.xyz / world_far.w - camera_pos);

    mat4 inv_model = inverse(uModel);
    vec3 ro = (inv_model * vec4(camera_pos, 1.0)).xyz;
    vec3 rd = normalize((inv_model * vec4(ray_dir, 0.0)).xyz);

    /* Slab Test: find where to start/stop marching */
    vec3 inv_dir = 1.0 / rd;
    vec3 t0 = (-0.5 - ro) * inv_dir, t1 = (0.5 - ro) * inv_dir;
    vec3 tmin = min(t0, t1), tmax = max(t0, t1);
    float tnear = max(max(tmin.x, tmin.y), tmin.z);
    float tfar  = min(min(tmax.x, tmax.y), tmax.z);
    if (tnear > tfar || tfar < 0.0) discard;
    tnear = max(tnear, 0.0);

    float dist = tfar - tnear;
    int steps = int(dist / 0.005) + 1;
    float step_size = dist / float(steps);

    /* Ray March: step and sample voxels along the ray */
    vec3 accum_color = vec3(0.0);
    float accum_alpha = 0.0;
    for (int i = 0; i < steps && accum_alpha < 0.99; i++)
    {
        vec3 tex_coord = ro + rd * (tnear + (float(i) + 0.5) * step_size) + 0.5;
        vec4 voxel = texture(uTexture, tex_coord);

        float step_alpha = 1.0 - exp(-voxel.a * step_size);
        float weight = 1.0 - accum_alpha;
        accum_color += weight * voxel.rgb * step_alpha;
        accum_alpha += weight * step_alpha;
    }

    accum_color = pow(accum_color / (accum_color + 1.0), vec3(1.0 / 2.2));
    fColor = vec4(accum_color, 1.0);
}
