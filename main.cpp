#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "LoadShaders.h"


GLuint VAOs[1];
GLuint Buffers[1];

GLuint pipeline;
GLuint vertexProgram;
GLuint fragmentProgram1;
GLuint fragmentProgram2;

// initialize the data
void initialize()
{

    GLfloat triangles[6][2] = {
        { -0.90f, -0.90f }, {  0.85f, -0.90f }, { -0.90f,  0.85f },
        {  0.90f, -0.85f }, {  0.90f,  0.90f }, { -0.85f,  0.90f } 
    };

    glGenVertexArrays(1, VAOs);
    glGenBuffers(1, Buffers);

    glBindVertexArray(VAOs[0]);

    glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangles), triangles, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    vertexProgram = LoadShaders(ShaderInfo(GL_VERTEX_SHADER, "../triangle.vert"));
    fragmentProgram1 = LoadShaders(ShaderInfo(GL_FRAGMENT_SHADER, "../triangle.frag"));
    fragmentProgram2 = LoadShaders(ShaderInfo(GL_FRAGMENT_SHADER, "../orange.frag"));

    // Create a program pipeline
    glGenProgramPipelines(1, &pipeline);
    glUseProgramStages(pipeline, GL_VERTEX_SHADER_BIT, vertexProgram);
}

// render the data
void render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(VAOs[0]);
    glBindProgramPipeline(pipeline);
    
    glUseProgramStages(pipeline, GL_FRAGMENT_SHADER_BIT, fragmentProgram1);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glUseProgramStages(pipeline, GL_FRAGMENT_SHADER_BIT, fragmentProgram2);
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