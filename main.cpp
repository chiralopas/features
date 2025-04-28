#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstring>
#include "LoadShaders.h"


GLuint VAOs[1];
GLuint Buffers[1];

// initialize the data
void initialize()
{
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

    ShaderInfo shaders[] =
    {
        {GL_VERTEX_SHADER, "../triangle.vert"},
        {GL_FRAGMENT_SHADER, "../triangle.frag"},
        {GL_NONE, NULL }
    };

    GLuint program = LoadShaders(shaders);
    glUseProgram(program);

    // uniform buffer object //
    
    /* Initialize uniform values in uniform block "Uniforms" */
    GLint uboSize;
    GLuint ubo;
    GLvoid* buffer;

    /* Find the uniform buffer index for "Uniforms", and determine the block’s sizes */
    GLuint uboIndex = glGetUniformBlockIndex(program,"Transformation");
    glGetActiveUniformBlockiv(program, uboIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &uboSize);
    
    buffer = malloc(uboSize);

    if (buffer == NULL) {
        fprintf(stderr, "Unable to allocate buffer\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        enum { Translation, Rotation, Scale, NumUniforms };
        
        /* values to be stored in the buffer object */
        GLfloat translation[] = { 0.0, 0.5, 0.0 };
        GLfloat rotation[] = { 90.0, 0.0, 0.0, 1.0 };
        GLfloat scale = 0.5;

        /* since we know the names of the uniforms in our block, make an array of those values */
        const char* names[NumUniforms] = {
            "translation",
            "rotation",
            "scale" };

        /* query the necessary attributes to determine where in the buffer we should write the values */
        GLuint indices[NumUniforms];
        GLint size[NumUniforms];
        GLint offset[NumUniforms];
        GLint type[NumUniforms];

        glGetUniformIndices(program, NumUniforms, names, indices);
        glGetActiveUniformsiv(program, NumUniforms, indices, GL_UNIFORM_OFFSET, offset);
        glGetActiveUniformsiv(program, NumUniforms, indices, GL_UNIFORM_SIZE, size);
        glGetActiveUniformsiv(program, NumUniforms, indices, GL_UNIFORM_TYPE, type);

        /* copy the uniform values into the buffer */
        memcpy((std::byte*)buffer + offset[Translation], &translation, size[Translation] * 3*sizeof(GLfloat));
        memcpy((std::byte*)buffer + offset[Rotation], &rotation, size[Rotation] * 4*sizeof(GLfloat));
        memcpy((std::byte*)buffer + offset[Scale], &scale, size[Scale] * 1*sizeof(GLfloat));
 
        /* create the uniform buffer object, initialize its storage, and associated it with the shader program */
        glGenBuffers(1, &ubo);
        glBindBuffer(GL_UNIFORM_BUFFER, ubo);
        glBufferData(GL_UNIFORM_BUFFER, uboSize, buffer, GL_STATIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo);
    }
}

// render the data
void render()
{
    glClear(GL_COLOR_BUFFER_BIT);

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