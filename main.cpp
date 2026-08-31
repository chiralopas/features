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
        // front
        -0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,
        // back
         0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,
        // right
         0.5f, -0.5f,  0.5f,     1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,     1.0f,  0.0f,  0.0f,
        // left
        -0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,
        // top
        -0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,
        // bottom
        -0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f
    };

    glGenVertexArrays(1, VAOs);
    glGenBuffers(1, Buffers);

    glBindVertexArray(VAOs[0]);

    glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); // position
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1); // normal

    ShaderInfo shaders[] =
    {
        {GL_VERTEX_SHADER, "../lighting.vert"},
        {GL_FRAGMENT_SHADER, "../lighting.frag"},
        {GL_NONE, NULL }
    };

    Program = LoadShaders(shaders);
    glUseProgram(Program);

    /* set static lighting */
    glUniform3f(glGetUniformLocation(Program, "uLightPos"), 2.0f, 0.5f, 3.0f);
    glUniform3f(glGetUniformLocation(Program, "uLightColor"), 1.0f, 1.0f, 1.0f);
}

// render the data
void render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    /* setup mvp matrices */
    mat4x4 model, view, projection;
    mat4x4_identity(model);
    mat4x4_translate(view, 0.0f, 0.0f, -3.0f);
    mat4x4_perspective(projection, 45.0f*(3.14159f/180.0f), 640.0f/480.f, 0.1f, 100.0f);

    /* model transformations */
    mat4x4_rotate_Y(model, model, -60.0f*(3.14159f/180.0f)*(float)glfwGetTime());

    /* update matrices in shader */
    glUniformMatrix4fv(glGetUniformLocation(Program, "uModel"),1,GL_FALSE,(GLfloat*)model);
    glUniformMatrix4fv(glGetUniformLocation(Program, "uView"),1,GL_FALSE,(GLfloat*)view);
    glUniformMatrix4fv(glGetUniformLocation(Program, "uProjection"),1,GL_FALSE,(GLfloat*)projection);

    // update normal matrix
    mat4x4 inv_model, transp_model;
    mat4x4_invert(inv_model, model);
    mat4x4_transpose(transp_model, inv_model);
    glUniformMatrix4fv(glGetUniformLocation(Program, "uNormalMat"),1,GL_FALSE,(GLfloat*)transp_model);

    // update view position
    mat4x4 inv_view;
    mat4x4_invert(inv_view, view);
    glUniform3fv(glGetUniformLocation(Program, "uViewPos"), 1, inv_view[3]);

    /* draw cube */
    glUniform3f(glGetUniformLocation(Program, "uObjectColor"), 1.0f, 0.5f, 0.3f);
    glBindVertexArray(VAOs[0]);
    for (int i = 0; i < 6; i++)
        glDrawArrays(GL_TRIANGLE_STRIP, i * 4, 4);
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