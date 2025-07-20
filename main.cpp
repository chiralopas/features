#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "LoadShaders.h"


GLuint VAOs[1];
GLuint Buffers[1];

// initialize the data
void initialize()
{

    GLfloat triangle1[3][2] = {
        { -0.2f, -0.2f }, {  0.0f,  0.2f }, { 0.2f, -0.2f }
    };

    GLfloat offsets[] = {
        -0.4f, 0.4f,
         0.4f, 0.4f,
        -0.4f,-0.4f,
         0.4f,-0.4f
    };

    glGenVertexArrays(1, VAOs);
    glGenBuffers(1, Buffers);

    glBindVertexArray(VAOs[0]);

    glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle1), triangle1, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // instanced vertex attribute setup
    GLuint instanceBuffer;
    glGenBuffers(1, &instanceBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(offsets), offsets, GL_STATIC_DRAW);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribDivisor(1, 1);

    ShaderInfo shaders[] =
    {
        {GL_VERTEX_SHADER, "../triangle.vert"},
        {GL_FRAGMENT_SHADER, "../triangle.frag"},
        {GL_NONE, NULL }
    };

    GLuint program = LoadShaders(shaders);
    glUseProgram(program);
}

// render the data
void render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(VAOs[0]);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 3, 4);
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