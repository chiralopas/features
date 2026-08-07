#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "LoadShaders.h"
#include <linmath.h/linmath.h>

GLuint VAOs[2];
GLuint Buffers[2];
GLuint Framebuffer;
GLuint TextureColorBuffer;
GLuint TextureDepthBuffer;
GLuint SceneProgram;
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

    /* create texture for depth data */
    glGenTextures(1, &TextureDepthBuffer);
    glBindTexture(GL_TEXTURE_2D, TextureDepthBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, 640, 480, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    /* attach depth attachment(texture image) to framebuffer */
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, TextureDepthBuffer, 0);

    /* check framebuffer completeness */
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR: Framebuffer is not complete!" << std::endl;

    /* unbind framebuffer */
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    glGenVertexArrays(2, VAOs);
    glGenBuffers(2, Buffers);

    
    /* initialize scene data (objects to render) */
    GLfloat vertices[] = {
        // triangle
        -1.0f, -0.4f,  4.0f,
         0.0f, -0.4f,  4.0f,
        -0.5f,  0.6f,  4.0f,
        // rectangle
         2.0f, -0.4f, -1.0f,
         2.0f,  0.3f, -1.0f,
         2.8f, -0.4f, -1.0f,
         2.8f,  0.3f, -1.0f,
        // plane
        -5.0f, -0.5f, -5.0f,
        -5.0f, -0.5f,  5.0f,
         5.0f, -0.5f, -5.0f,
         5.0f, -0.5f,  5.0f
    };

    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    ShaderInfo sceneShaders[] = {
        {GL_VERTEX_SHADER, "../scene.vert"},
        {GL_FRAGMENT_SHADER, "../scene.frag"},
        {GL_NONE, NULL}
    };

    SceneProgram = LoadShaders(sceneShaders);


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
    /* FIRST PASS: Render scene to framebuffer object */
    glBindFramebuffer(GL_FRAMEBUFFER, Framebuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glUseProgram(SceneProgram); 

    mat4x4 model, view, proj;
    mat4x4_identity(model);
    mat4x4_translate(view, 0.0f, 0.0f, -7.0f);
    mat4x4_perspective(proj, 45.0f * 3.14159f / 180.0f, 640.0f / 480.0f, 0.1f, 100.0f);

    glUniformMatrix4fv(glGetUniformLocation(SceneProgram, "uModel"),1,GL_FALSE,(GLfloat*)model);
    glUniformMatrix4fv(glGetUniformLocation(SceneProgram, "uView"),1,GL_FALSE,(GLfloat*)view);
    glUniformMatrix4fv(glGetUniformLocation(SceneProgram, "uProjection"),1,GL_FALSE,(GLfloat*)proj);
    
    glBindVertexArray(VAOs[0]);
    glUniform3f(glGetUniformLocation(SceneProgram, "uColor"), 0.8f, 0.2f, 0.2f);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawArrays(GL_TRIANGLE_STRIP, 3, 4);

    glUniform3f(glGetUniformLocation(SceneProgram, "uColor"), 0.5f, 0.4f, 0.3f);
    glDrawArrays(GL_TRIANGLE_STRIP, 7, 4);

    /* SECOND PASS: Render textures to screen with fog blend */
    glBindFramebuffer(GL_FRAMEBUFFER, 0); // default framebuffer
    glDisable(GL_DEPTH_TEST);
    glUseProgram(ScreenProgram);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, TextureColorBuffer);
    glUniform1i(glGetUniformLocation(ScreenProgram, "uScreenColor"), 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, TextureDepthBuffer);
    glUniform1i(glGetUniformLocation(ScreenProgram, "uScreenDepth"), 1);

    glBindVertexArray(VAOs[1]);
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