#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "LoadShaders.h"


GLuint VAOs[1];
GLuint Buffers[1];

GLuint pack_color(short red, short green, short blue, short alpha)
{
    GLuint color = 0;
    color = color | (alpha << 30);
    color = color | (blue << 20);
    color = color | (green << 10);
    color = color | (red << 0);

    return color;
}

// initialize the data
void initialize()
{
    GLfloat triangle1[3][2] = {
        {-0.5f,-0.5f}, { 0.0f, 0.5f}, { 0.5f,-0.5f}
    };

    GLuint colors[] = {
        pack_color(1, 0, 0, 1),
        pack_color(0, 1, 0, 1),
        pack_color(0, 0, 1, 1)
    };
    

    glGenVertexArrays(1, VAOs);
    glGenBuffers(2, Buffers);

    glBindVertexArray(VAOs[0]);

    glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle1), triangle1, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    glBindBuffer(GL_ARRAY_BUFFER, Buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    glVertexAttribPointer(1, 4, GL_UNSIGNED_INT_2_10_10_10_REV, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);

    
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