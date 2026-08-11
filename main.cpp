#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "LoadShaders.h"

// GL_EXT_texture_compression_s3tc
#define GL_COMPRESSED_RGB_S3TC_DXT1_EXT 0x83F0

#include "tools/tex_header.h"

GLuint VAOs[1];
GLuint Buffers[1];
GLuint Texture;
GLuint Program;

// initialize the data
void initialize()
{
    GLfloat triangle1[] = {
         //position   //texcoord
        -0.5f,-0.5f,  0.0f, 0.0f,
         0.0f, 0.5f,  0.5f, 1.0f,
         0.5f,-0.5f,  1.0f, 0.0f
    };

    glGenVertexArrays(1, VAOs);
    glGenBuffers(1, Buffers);

    glBindVertexArray(VAOs[0]);

    glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle1), triangle1, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); //position

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(2*sizeof(float)));
    glEnableVertexAttribArray(1); //texcoord

    /* setup texture */
    glGenTextures(1, &Texture);
    glBindTexture(GL_TEXTURE_2D, Texture);

    /* load data from compressed texture */
    FILE* tf = fopen("../grad4k.tex", "rb");
    tex_header hdr;
    fread(&hdr, 1, sizeof(hdr), tf);
    unsigned char* data = new unsigned char[hdr.size];
    fread(data, 1, hdr.size, tf);
    fclose(tf);

    glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGB_S3TC_DXT1_EXT,
        hdr.width, hdr.height, 0, hdr.size, data);
    delete[] data;
    
    /* set texture parameters */
    // filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

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

    /* bind texture to slot 0 */
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Texture);

    /* sample texture in shader from slot 0 */
    glUniform1i(glGetUniformLocation(Program, "uTexture"), 0);

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