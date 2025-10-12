#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "LoadShaders.h"


GLuint VAOs[1];
GLuint Buffers[1];
GLuint Program;

// initialize the data
void initialize()
{
    GLfloat triangles[6][2] = {
        {-0.5f,-0.3f}, {-0.1f, 0.5f}, { 0.3f,-0.3f},
        {-0.3f,-0.5f}, { 0.1f, 0.3f}, { 0.5f,-0.5f}
    };

    glGenVertexArrays(1, VAOs);
    glGenBuffers(1, Buffers);

    glBindVertexArray(VAOs[0]);

    glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangles), triangles, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
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

    /* enable blending */
    glEnable(GL_BLEND);
    /* blending equation:
       result = scolor x sfactor + dcolor x dfactor */
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClear(GL_COLOR_BUFFER_BIT);

    /* draw red triangle */
    glUniform4f(glGetUniformLocation(Program, "uColor"), 1.0f, 0.0f, 0.0f, 1.0f);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    /* draw green triangle */
    glUniform4f(glGetUniformLocation(Program, "uColor"), 0.0f, 1.0f, 0.0f, 0.6f);
    glDrawArrays(GL_TRIANGLES, 3, 3);
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