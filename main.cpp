#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "LoadShaders.h"

const int PARTICLE_COUNT = 100;

GLuint VAOs[2];
GLuint Buffers[2];

GLuint updateProgram;
GLuint renderProgram;
int currentBuffer = 0;


// initialize the data
void initialize()
{
    /* build initial particle data(px, py, vx, vy) */
    float particles[PARTICLE_COUNT * 4];
    for (int i = 0; i < PARTICLE_COUNT; i++)
    {
        float px = 0.0f;
        float py = 0.0f;

        // random direction: in range of -0.5 to 0.5
        float vx = ((float)rand() / RAND_MAX) - 0.5f;
        float vy = ((float)rand() / RAND_MAX) - 0.5f;

        particles[i*4 + 0] = px;
        particles[i*4 + 1] = py;
        particles[i*4 + 2] = vx;
        particles[i*4 + 3] = vy;
    }


    glGenVertexArrays(2, VAOs);
    glGenBuffers(2, Buffers); // for buffer ping-ponging

    /* vertex layout for buffer with particle data*/
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(particles), particles, GL_DYNAMIC_COPY);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); // position
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(2*sizeof(float))); 
    glEnableVertexAttribArray(1); // velocity

    /* vertex layout similar to particle but without data*/
    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, Buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(particles), nullptr, GL_DYNAMIC_COPY);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); // position
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(2*sizeof(float))); 
    glEnableVertexAttribArray(1); // velocity


    /* setup 'update' program (uses transform feedback) */
    ShaderInfo updateShaders[] =
    {
        {GL_VERTEX_SHADER, "../update.vert"},
        {GL_NONE, NULL }
    };

    updateProgram = LoadShaders(updateShaders);

    /* configure transform feedback with shader before linking */
    // these names must exactly match the 'out' variables in update.vert
    const char* varyings[] = { "outPosition", "outVelocity" };
    glTransformFeedbackVaryings(updateProgram, 2, varyings, GL_INTERLEAVED_ATTRIBS);

    /* link after setting varyings (this order is mandatory) */
    glLinkProgram(updateProgram);
    // check link status
    GLint linkStatus = 0;
    glGetProgramiv(updateProgram, GL_LINK_STATUS, &linkStatus);
    if (!linkStatus)
    {
        char log[512];
        glGetProgramInfoLog(updateProgram, 512, NULL, log);
        std::cerr << "Update program link error:\n" << log << std::endl;
    }


    /* setup 'render' program (draws to screen) */
    ShaderInfo renderShaders[] =
    {
        {GL_VERTEX_SHADER, "../render.vert"},
        {GL_FRAGMENT_SHADER, "../render.frag"},
        {GL_NONE, NULL}
    };

    renderProgram = LoadShaders(renderShaders);
}

// render the data
void render(float deltaTime)
{
    /* src: read from, dst: write to*/
    int src = currentBuffer;
    int dst = 1 - currentBuffer; // toggles between 0 and 1

    /* PASS 1: UPDATE (physics via transform feedback) */
    glUseProgram(updateProgram);

    /* make movement independent of frame-rate */
    glUniform1f(glGetUniformLocation(updateProgram, "uDeltaTime"), deltaTime);

    /* bind destination buffer with slot 0 of transform feedback(default)*/
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, Buffers[dst]);

    /* read from source buffer (contains current particle state) */
    glBindVertexArray(VAOs[src]);
    glBeginTransformFeedback(GL_POINTS); // begin capturing
        glDrawArrays(GL_POINTS, 0, PARTICLE_COUNT);
    glEndTransformFeedback();
    

    /* PASS 2: RENDER (draw updated particles to screen) */
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(renderProgram);

    /* read from destination buffer (contains newly updated positions) */
    glBindVertexArray(VAOs[dst]);
    glDrawArrays(GL_POINTS, 0, PARTICLE_COUNT);

    /* swap buffers for next frame */
    currentBuffer = dst;
}


int main()
{
    glfwInit();

    GLFWwindow* window = glfwCreateWindow(640, 480, "sandbox", NULL, NULL);
    glfwMakeContextCurrent(window);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    initialize();

    float lastTime = (float)glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {
        float currentTime = (float)glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        render(deltaTime);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}