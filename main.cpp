#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "LoadShaders.h"


GLuint VAOs[1];
GLuint Buffers[1];

// initialize the data
void initialize()
{
    GLfloat data[27][2] = {
        // triangle
        {-0.5f,-0.5f}, { 0.0f, 0.5f}, { 0.5f,-0.5f},

        // rectangular frame
        {-0.2f, 0.0f}, { 0.2f, 0.0f}, { 0.2f,-0.1f},
        { 0.2f,-0.1f}, {-0.2f,-0.1f}, {-0.2f, 0.0f},
        {-0.2f,-0.3f}, { 0.2f,-0.3f}, { 0.2f,-0.4f},
        { 0.2f,-0.4f}, {-0.2f,-0.4f}, {-0.2f,-0.3f},
        {-0.2f,-0.1f}, {-0.1f,-0.1f}, {-0.1f,-0.3f},
        {-0.1f,-0.3f}, {-0.2f,-0.3f}, {-0.2f,-0.1f},
        { 0.1f,-0.1f}, { 0.2f,-0.1f}, { 0.2f,-0.3f},
        { 0.2f,-0.3f}, { 0.1f,-0.3f}, { 0.1f,-0.1f}
    };

    glGenVertexArrays(1, VAOs);
    glGenBuffers(1, Buffers);

    glBindVertexArray(VAOs[0]);

    glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

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
    glBindVertexArray(VAOs[0]);

    /* enable stencil test */
    glEnable(GL_STENCIL_TEST);
    glClearStencil(0x00);
    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    /* setup test action to update stencil buffer */
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glStencilFunc(GL_ALWAYS, 0x01, 0x01); // stencil test
    glStencilMask(0x01); // enable writes for rightmost bit
    glDrawArrays(GL_TRIANGLES, 3, 24);

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glStencilFunc(GL_EQUAL, 0x01, 0x01); // stencil test
    glStencilMask(0x00); // disable writes for all bits
    glDrawArrays(GL_TRIANGLES, 0, 3);

    /* disable stencil test */
    glDisable(GL_STENCIL_TEST);
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