#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "LoadShaders.h"
#include <linmath.h/linmath.h>


GLuint VAOs[1];
GLuint Buffers[1];

GLuint Program;

// initialize the data
void initialize()
{
    GLfloat vertices[] = {
        // triangle
        -0.5f, -0.5f,  0.0f,
         0.5f, -0.5f,  0.0f,
         0.0f,  0.5f,  0.0f,
        // rectangle
        -0.4f, -0.5f, -0.0000001f,
        -0.4f,  0.2f, -0.0000001f,
         0.4f, -0.5f, -0.0000001f,
         0.4f,  0.2f, -0.0000001f
    };

    glGenVertexArrays(1, VAOs);
    glGenBuffers(1, Buffers);

    glBindVertexArray(VAOs[0]);

    glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    ShaderInfo shaders[] =
    {
        {GL_VERTEX_SHADER, "../triangle.vert"},
        {GL_FRAGMENT_SHADER, "../triangle.frag"},
        {GL_NONE, NULL }
    };

    Program = LoadShaders(shaders);
    glUseProgram(Program);
}

// render the data
void render()
{
    glBindVertexArray(VAOs[0]);

    /* enable depth test */
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* setup mvp matrices*/
    mat4x4 model, view, projection;
    mat4x4_identity(model);
    mat4x4_translate(view, 0.0f, 0.0f, -3.0f);
    mat4x4_perspective(projection, 45.0f * (3.14159f / 180.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    /* model transformations */
    mat4x4_rotate_Y(model, model, (float)glfwGetTime());

    /* update matrices in shader */
    glUniformMatrix4fv(glGetUniformLocation(Program, "uModel"), 1, GL_FALSE, (GLfloat*)model);
    glUniformMatrix4fv(glGetUniformLocation(Program, "uView"), 1, GL_FALSE, (GLfloat*)view);
    glUniformMatrix4fv(glGetUniformLocation(Program, "uProjection"), 1, GL_FALSE, (GLfloat*)projection);

    /* draw triangle */
    glUniform3f(glGetUniformLocation(Program, "uColor"), 0.4f, 0.4f, 0.8f);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    /* draw rectangle */
    glUniform3f(glGetUniformLocation(Program, "uColor"), 0.8f, 0.4f, 0.0f);
    glDrawArrays(GL_TRIANGLE_STRIP, 3, 4);
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