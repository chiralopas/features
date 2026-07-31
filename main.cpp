#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "LoadShaders.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

GLuint VAOs[1];
GLuint Buffers[1];
GLuint Texture;
GLuint Program;
const int FrameCount = 4;

// initialize the data
void initialize()
{
    GLfloat quad[] = {
         //position   //texcoord
        -0.5f, -0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  1.0f, 1.0f
    };

    glGenVertexArrays(1, VAOs);
    glGenBuffers(1, Buffers);

    glBindVertexArray(VAOs[0]);

    glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); //position

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(2*sizeof(float)));
    glEnableVertexAttribArray(1); //texcoord

    /* setup array texture */
    glGenTextures(1, &Texture);
    glBindTexture(GL_TEXTURE_2D_ARRAY, Texture);
    
    const char* framePaths[FrameCount] = {
        "../res/attack_1.png",
        "../res/attack_2.png",
        "../res/attack_3.png",
        "../res/attack_4.png"
    };

    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    stbi_info(framePaths[0], &width, &height, &channels);

    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8, width, height, FrameCount,
             0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    for (int i = 0; i < FrameCount; i++)
    {
        unsigned char* data = stbi_load(framePaths[i], &width, &height, &channels, 0);
        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, width, height, 1,
            GL_RGBA, GL_UNSIGNED_BYTE, data);
    }

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

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
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    int slice = int(glfwGetTime() * 4.0) % FrameCount;
    glUniform1i(glGetUniformLocation(Program, "uSlice"), slice);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, Texture);
    glUniform1i(glGetUniformLocation(Program, "uTexture"), 0);

    glBindVertexArray(VAOs[0]);
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