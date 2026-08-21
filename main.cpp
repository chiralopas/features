#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "LoadShaders.h"
#include <linmath.h/linmath.h>


GLuint VAOs[1];
GLuint Buffers[1];
GLuint Texture;
GLuint Program;

// initialize the data
void initialize()
{
    GLfloat vertices[] = {
        // positions          // texcoords
        -2.0f, 0.0f,-5.0f,    0.0f, 0.0f,
        -2.0f, 0.0f, 5.0f,    0.0f, 1.0f,
         2.0f, 0.0f,-5.0f,    1.0f, 0.0f,
         2.0f, 0.0f, 5.0f,    1.0f, 1.0f
    };

    glGenVertexArrays(1, VAOs);
    glGenBuffers(1, Buffers);

    glBindVertexArray(VAOs[0]);

    glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); //position
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1); //texcoord

    /* procedurally create a texture with mipmap */
    glGenTextures(1, &Texture);
    glBindTexture(GL_TEXTURE_2D, Texture);

    unsigned int colors[] = { 
        0x0000FF, 0x00FF00, 0xFF0000, 0x00FFFF, 0xFF00FF, 0xFFFF00, 0xFFFFFF, 0xFFA500, 0x8B00FF
    };

    for (int i = 0; i < 9; i++)
    {
        int n = 0;
        unsigned int data[256 * 256];

        for (int j = 0; j < (256 >> i); j++)
        {
            for (int k = 0; k < (256 >> i); k++)
            {
                data[n] = colors[i];
                n++;
            }
        }
        glTexImage2D(GL_TEXTURE_2D, i, GL_RGBA8, 256 >> i, 256 >> i, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    /* setting lod_bias for mipmap */
    // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, 1.0f);

    /* setting clamp for mip level */
    // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_LOD, 0.0f);
    // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_LOD, 2.0f);

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

    /* setup mvp matrices */
    mat4x4 model, view, projection;
    mat4x4_identity(model);
    mat4x4_translate(view, 0.0f, 0.0f, -5.0f);
    mat4x4_perspective(projection, 45.0f*(3.14159f/180.0f), 640.0f/480.f, 0.1f, 100.0f);

    mat4x4_rotate_X(model, model, 7.2f*(3.14159f/180.0f));

    glUniformMatrix4fv(glGetUniformLocation(Program, "uModel"),1,GL_FALSE,(GLfloat*)model);
    glUniformMatrix4fv(glGetUniformLocation(Program, "uView"),1,GL_FALSE,(GLfloat*)view);
    glUniformMatrix4fv(glGetUniformLocation(Program, "uProjection"),1,GL_FALSE,(GLfloat*)projection);

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