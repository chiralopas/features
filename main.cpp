#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "LoadShaders.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <linmath.h/linmath.h>

GLuint VAOs[1];
GLuint Buffers[1];
GLuint Texture;
GLuint Program;

void initialize()
{
    GLfloat points[] = {
        -0.5f, 0.2f,
         0.5f, 0.2f
    };

    glGenVertexArrays(1, VAOs);
    glGenBuffers(1, Buffers);

    glBindVertexArray(VAOs[0]);

    glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); //position

    /* setup texture */
    glGenTextures(1, &Texture);
    glBindTexture(GL_TEXTURE_2D, Texture);

    /* load image data into texture */
    int width, height, components;
    stbi_set_flip_vertically_on_load(true); // opengl reads texture from bottom left
    unsigned char *data = stbi_load("../star.png", &width, &height, &components, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    /* set texture parameters */
    // filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    /* set sprite's start point to be same as texture's */
    glPointParameteri(GL_POINT_SPRITE_COORD_ORIGIN, GL_LOWER_LEFT);

    ShaderInfo shaders[] =
    {
        {GL_VERTEX_SHADER, "../particle.vert"},
        {GL_FRAGMENT_SHADER, "../particle.frag"},
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

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);
    glEnable(GL_PROGRAM_POINT_SIZE);

    /* makes 'gl_PointCoord' valid in compat profile */
    glEnable(GL_POINT_SPRITE);
    
    /* draw points */
    glBindVertexArray(VAOs[0]);
    glUniform1f(glGetUniformLocation(Program, "uTime"), (float)glfwGetTime());
    glDrawArrays(GL_POINTS, 0, 2);
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