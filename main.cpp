#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "LoadShaders.h"
#include <linmath.h/linmath.h>

GLuint VAOs[2];
GLuint Buffers[2];
GLuint Program;

// initialize the data
void initialize()
{
    GLfloat triangle1[] = {
        -0.5f, -0.5f,  0.0f,
         0.0f,  0.5f,  0.0f,
         0.5f, -0.5f,  0.0f
    };

    GLfloat triangle2[] = {
        -0.5f, -0.5f, -0.2f,
         0.0f,  0.5f, -0.2f,
         0.5f, -0.5f, -0.2f
    };

    glGenVertexArrays(2, VAOs);
    glGenBuffers(2, Buffers);

    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle1), triangle1, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, Buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle2), triangle2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
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

void render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    /* activate user clip plane slot 0 */
    glEnable(GL_CLIP_DISTANCE0);

    /* setup mvp matrices */
    mat4x4 model, view, projection;
    mat4x4_identity(model);
    mat4x4_translate(view, 0.0f, 0.0f, -3.0f);
    mat4x4_perspective(projection, 45.0f*(3.14159f/180.0f), 640.0f/480.f, 0.1f, 100.0f);

    /* update matrices in shader */
    glUniformMatrix4fv(glGetUniformLocation(Program, "uModel"),1,GL_FALSE,(GLfloat*)model);
    glUniformMatrix4fv(glGetUniformLocation(Program, "uView"),1,GL_FALSE,(GLfloat*)view);
    glUniformMatrix4fv(glGetUniformLocation(Program, "uProjection"),1,GL_FALSE,(GLfloat*)projection);

    /* Clip Plane: ax + by + cz + d = 0, i.e. vec4(a,b,c,d)*/
    // mostly faces x, slightly tilted into z
    glUniform4f(glGetUniformLocation(Program, "uClipPlane"), 1.0f, 0.0f, -0.2f, -0.1f);

    /* draw red triangle */
    glUniform4f(glGetUniformLocation(Program, "uColor"), 0.8f, 0.2f, 0.2f, 1.0f);
    glBindVertexArray(VAOs[0]);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    /* draw blue triangle */
    glUniform4f(glGetUniformLocation(Program, "uColor"), 0.2f, 0.4f, 0.8f, 1.0f);
    glBindVertexArray(VAOs[1]);
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