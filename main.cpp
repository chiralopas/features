#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "LoadShaders.h"


GLuint VAOs[1];
GLuint Buffers[1];
GLuint Framebuffer;
GLuint Renderbuffer;
GLuint TriangleProgram;


// initialize the data
void initialize()
{
    /* create framebuffer */
    glGenFramebuffers(1, &Framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, Framebuffer);

    /* create renderbuffer for color data */
    glGenRenderbuffers(1, &Renderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, Renderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGB8, 640, 480);

    /* attach color attachment(renderbuffer) to framebuffer */
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, Renderbuffer);

    /* check framebuffer completeness */
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR: Framebuffer is not complete!" << std::endl;

    /* unbind framebuffer */
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    glGenVertexArrays(1, VAOs);
    glGenBuffers(1, Buffers);
    
    
    /* initialize triangle data (to render to framebuffer) */
    GLfloat triangle1[3][2] = {
        {-0.5f,-0.5f}, { 0.0f, 0.5f}, { 0.5f,-0.5f}
    };

    glGenVertexArrays(1, VAOs);
    glGenBuffers(1, Buffers);

    glBindVertexArray(VAOs[0]);

    glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle1), triangle1, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    ShaderInfo triangleShaders[] = {
        {GL_VERTEX_SHADER, "../triangle.vert"},
        {GL_FRAGMENT_SHADER, "../triangle.frag"},
        {GL_NONE, NULL}
    };

    TriangleProgram = LoadShaders(triangleShaders);
}

// render the data
void render()
{
    /* FIRST PASS: Render triangle to framebuffer with renderbuffer */
    glBindFramebuffer(GL_FRAMEBUFFER, Framebuffer);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(TriangleProgram);
    glBindVertexArray(VAOs[0]);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    /* SECOND PASS: Blit framebuffer contents to default framebuffer */
    glBindFramebuffer(GL_READ_FRAMEBUFFER, Framebuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBlitFramebuffer(0, 0, 640, 480, 0, 0, 640, 480, GL_COLOR_BUFFER_BIT, GL_LINEAR);
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
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