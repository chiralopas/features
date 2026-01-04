#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "LoadShaders.h"

GLuint VAOs[2];
GLuint Buffers[2];
GLuint Framebuffer;
GLuint TextureColorBuffer;
GLuint TriangleProgram;
GLuint ScreenProgram;


// initialize the data
void initialize()
{
    /* create framebuffer */
    glGenFramebuffers(1, &Framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, Framebuffer);

    /* create texture for color data */
    glGenTextures(1, &TextureColorBuffer);
    glBindTexture(GL_TEXTURE_2D, TextureColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 640, 480, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    /* attach color attachment(texture image) to framebuffer*/
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TextureColorBuffer, 0);

    /* check framebuffer completeness */
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR: Framebuffer is not complete!" << std::endl;

    /* unbind framebuffer */
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    glGenVertexArrays(2, VAOs);
    glGenBuffers(2, Buffers);
    
    
    /* initialize triangle data (object to render) */
    GLfloat triangle1[3][2] = {
        {-0.5f,-0.5f}, { 0.0f, 0.5f}, { 0.5f,-0.5f}
    };

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


    /* initialize screen quad (rendering target) */
    GLfloat screenQuad[] = {
        // positions   // texcoords
        -1.0f, -1.0f,  0.0f, 0.0f,
        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, Buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(screenQuad), screenQuad, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(2*sizeof(float)));
    glEnableVertexAttribArray(1);

    ShaderInfo screenShaders[] = {
        {GL_VERTEX_SHADER, "../screen.vert"},
        {GL_FRAGMENT_SHADER, "../screen.frag"},
        {GL_NONE, NULL}
    };

    ScreenProgram = LoadShaders(screenShaders);
}

// render the data
void render()
{
    /* FIRST PASS: Render triangle to framebuffer object */
    glBindFramebuffer(GL_FRAMEBUFFER, Framebuffer);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(TriangleProgram);
    glBindVertexArray(VAOs[0]);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    /* SECOND PASS: Render texture to screen */
    glBindFramebuffer(GL_FRAMEBUFFER, 0); // 0 is default framebuffer
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(ScreenProgram);
    glBindVertexArray(VAOs[1]);
    glBindTexture(GL_TEXTURE_2D, TextureColorBuffer);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
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