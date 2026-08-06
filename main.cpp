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

// initialize the data
void initialize()
{
    GLfloat skybox[] = {
        // front
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        // back
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        // right
         1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
        // left
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        // top
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        // bottom
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    glGenVertexArrays(1, VAOs);
    glGenBuffers(1, Buffers);
    
    glBindVertexArray(VAOs[0]);
    
    glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skybox), skybox, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    /* setup cubemap texture */
    glGenTextures(1, &Texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, Texture);

    const char* faces[6] = {
        "../res/right.jpg",
        "../res/left.jpg",
        "../res/top.jpg",
        "../res/bottom.jpg",
        "../res/front.jpg",
        "../res/back.jpg"
    };

    int width, height, channels;
    for (int i = 0; i < 6; i++)
    {
        unsigned char *data = stbi_load(faces[i], &width, &height, &channels, 0);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
            0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    ShaderInfo shaders[] = {
        {GL_VERTEX_SHADER, "../skybox.vert"},
        {GL_FRAGMENT_SHADER, "../skybox.frag"},
        {GL_NONE, NULL }
    };

    Program = LoadShaders(shaders);
    glUseProgram(Program);
}

// render the data
void render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    
    /* draw skybox */
    glDepthFunc(GL_LEQUAL);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, Texture);
    glUniform1i(glGetUniformLocation(Program, "uTexture"), 0);

    // only view and projection needed
    mat4x4 view, proj;
    mat4x4_identity(view);
    mat4x4_perspective(proj, 45.0f * 3.14159f / 180.0f, 640.0f / 480.0f, 0.1f, 100.0f);

    // rotate around to see the environment
    // mat4x4_rotate_Y(view, view, (float)glfwGetTime()*0.5f);

    glUniformMatrix4fv(glGetUniformLocation(Program, "uView"), 1, GL_FALSE, (GLfloat*)view);
    glUniformMatrix4fv(glGetUniformLocation(Program, "uProjection"), 1, GL_FALSE, (GLfloat*)proj);
    
    glBindVertexArray(VAOs[0]);
    for (int i = 0; i < 6; i++)
        glDrawArrays(GL_TRIANGLE_STRIP, i * 4, 4);

    glDepthFunc(GL_LESS);
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