#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "LoadShaders.h"
#include <linmath.h/linmath.h>

#define STB_PERLIN_IMPLEMENTATION
#include <stb_perlin.h>

GLuint VAOs[1];
GLuint Texture;
GLuint Program;

// initialize the data
void initialize()
{
    glGenVertexArrays(1, VAOs);

    /* setup 3d texture */
    glGenTextures(1, &Texture);
    glBindTexture(GL_TEXTURE_3D, Texture);

    /* generate cloud density as 3D fbm noise */
    const int dx = 100, dy = 100, dz = 100;
    static float data[dx * dy * dz * 4];
    for (int z = 0; z < dz; z++)
        for (int y = 0; y < dy; y++)
            for (int x = 0; x < dx; x++)
            {
                float u = (float)x / dx, v = (float)y / dy, w = (float)z / dz;
                float cloud = stb_perlin_fbm_noise3(u * 2.0f, v * 2.0f, w * 2.0f, 2.0f, 0.7f, 6);
                int idx = (z * dy + y) * dx + x;
                data[idx * 4 + 0] = cloud;
                data[idx * 4 + 1] = cloud;
                data[idx * 4 + 2] = cloud;
                data[idx * 4 + 3] = cloud;
            }
    glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA16F, dx, dy, dz, 0, GL_RGBA, GL_FLOAT, data);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    ShaderInfo shaders[] =
    {
        {GL_VERTEX_SHADER, "../cloud.vert"},
        {GL_FRAGMENT_SHADER, "../cloud.frag"},
        {GL_NONE, NULL }
    };

    Program = LoadShaders(shaders);
    glUseProgram(Program);
}

// render the data
void render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_3D, Texture);
    glUniform1i(glGetUniformLocation(Program, "uTexture"), 0);


    mat4x4 model, view, proj;
    mat4x4_identity(model);
    mat4x4_rotate_Y(model, model, (float)glfwGetTime() * 1.2f);
    mat4x4_translate(view, 0.0f, 0.0f, -3.0f);
    mat4x4_perspective(proj, 45.0f * 3.14159f / 180.0f, 640.0f / 480.0f, 0.1f, 100.0f);

    glUniformMatrix4fv(glGetUniformLocation(Program, "uModel"), 1, GL_FALSE, (GLfloat*)model);
    glUniformMatrix4fv(glGetUniformLocation(Program, "uView"), 1, GL_FALSE, (GLfloat*)view);
    glUniformMatrix4fv(glGetUniformLocation(Program, "uProjection"), 1, GL_FALSE, (GLfloat*)proj);

    glBindVertexArray(VAOs[0]);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

int main()
{
    glfwInit();

    GLFWwindow* window = glfwCreateWindow(640, 480, "sandbox", NULL, NULL);
    glfwMakeContextCurrent(window);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    initialize();

    while (!glfwWindowShouldClose(window))
    {
        render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}