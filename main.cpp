#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "LoadShaders.h"


GLuint VAOs[1];
GLuint Buffers[1];

// initialize the data
void initialize()
{

    GLfloat positions[] = {
        -0.5f,-0.5f,
         0.0f, 0.5f,
         0.5f,-0.5f
    };

    GLfloat colors[] = {
        1.0, 0.0, 0.0, 1.0,
        0.0, 1.0, 0.0, 1.0,
        0.0, 0.0, 1.0, 1.0
    };

    glGenVertexArrays(1, VAOs);
    glBindVertexArray(VAOs[0]);

    glGenBuffers(1, Buffers);
    glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
    
    /* Allocate space for it positions and colors */
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions) + sizeof(colors), NULL, GL_STATIC_DRAW);
    
    /* Put "positions" at offset zero in the buffer */
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(positions), positions);

    /* Put "colors" at an offset in the buffer equal to the filled size of the buffer so far,
    i.e. sizeof(positions)*/
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(positions), sizeof(colors), colors);


    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(sizeof(positions)));
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