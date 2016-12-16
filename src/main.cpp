#include <cstdint>
#include <ctime>
#include <iostream>
#include <random>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

//#include "lodepng.h"
#include "Shader.h"

#define GLSL(x) #x

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

int main(int argc, char** argv)
{
    // Setup GLFW
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    int width, height;
    GLFWwindow *window;
#define FULLSCREEN 0
#if FULLSCREEN
    width = 1920;
    height = 1080;
    window = glfwCreateWindow(width, height, "Shadertoy Screensaver", glfwGetPrimaryMonitor(), nullptr);
#else
    width = 800;
    height = 600;
    window = glfwCreateWindow(width, height, "Shadertoy Screensaver", nullptr, nullptr);
#endif

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    int count = 0;
    glfwGetMonitors(&count);
    std::cout << "Number of monitors: " << count << "\n";

    // Load in OGL functions
    if (!gladLoadGL()) {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        return -1;
    }

    // Check if we are uses a default shader or a passed in shader
    std::string shadertoyShaderPath("shaders/hexes.frag");
    std::cout << "argc: " << argc << "\n";
    if (argc > 1) {
        shadertoyShaderPath = argv[1];
    }
    std::cout << "Using shader path: " << shadertoyShaderPath << "\n";
    Shader s("shaders/simple.vert", shadertoyShaderPath);
    if(!s.compile()) {
        std::cout << "Some error happened while creating shaders\n";
        return -1;
    }

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // load in vertex data
    float vertices[] = {
          /* position */     /* texcoords */
        -1.0, -1.0, 0.0,    0.0, 0.0,
        1.0, -1.0, 0.0,     1.0, 0.0,
        1.0, 1.0, 0.0,      1.0, 1.0,
        -1.0, 1.0, 0.0,     0.0, 1.0,
    };
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // TexCoord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // load in index data
    GLuint vertex_indices[] = {
        0, 1, 2,
        2, 3, 0,
    };
    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertex_indices), vertex_indices, GL_STATIC_DRAW);

    // get uniform locations
    GLint textureDataSamplerLoc = glGetUniformLocation(s.getShaderProgram(), "textureData");
    GLint iResolutionUniformLoc = glGetUniformLocation(s.getShaderProgram(), "iResolution");
    GLint iGlobalTimeLoc = glGetUniformLocation(s.getShaderProgram(), "iGlobalTime");
    GLint iMouseUniformLoc = glGetUniformLocation(s.getShaderProgram(), "iMouse");

    // main loop
    double mouseX, mouseY, mouseZ, mouseW;
    mouseX = mouseY = mouseZ = mouseW = 0.0;
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        // Activate shader
        glUseProgram(s.getShaderProgram());

        // set uniforms
        glUniform1i(textureDataSamplerLoc, 0);
        glUniform3f(iResolutionUniformLoc, (float)width, (float)height, 0.0f);
        glUniform1f(iGlobalTimeLoc, (float)glfwGetTime() + 5.0f);

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS
            || glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
            mouseZ = 1.0f;
            glfwGetCursorPos(window, &mouseX, &mouseY);
        }
        glUniform4f(iMouseUniformLoc, (float)mouseX, (float)mouseY, (float)mouseZ, (float)mouseW);

        // Draw container
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}
