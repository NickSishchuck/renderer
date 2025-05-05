#include <cmath>
#include <cstdlib>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../include/shaderClass.h"
#include "../include/VAO.h"
#include "../include/VBO.h"
#include "../include/EBO.h"
#include "../include/Logger.h"

// Error callback for GLFW
void errorCallback(int error, const char* description) {
    LOG_ERROR(std::string("GLFW Error ") + std::to_string(error) + ": " + description);
}

int main() {
    // Initialize Logger first
    Logger* logger = Logger::getInstance();
    logger->enableColors(true); // Enable colored output for Linux terminal

    // Optional: Explicitly set the base path to strip from file paths
    // This will be auto-detected if not set
    // logger->setBasePath("/home/nick/Projects/CPP/Renderer/");

    if (!logger->init()) {
        std::cerr << "Failed to initialize logger!" << std::endl;
        return -1;
    }

    LOG_INFO("Application starting...");

    // Initialize GLFW
    if (!glfwInit()) {
        LOG_FATAL("Failed to initialize GLFW");
        return -1;
    }

    LOG_DEBUG("GLFW initialized successfully");
    glfwSetErrorCallback(errorCallback);

    // Set OpenGL version and profile
    LOG_DEBUG("Setting up OpenGL context...");
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //Triangle
    LOG_DEBUG("Creating vertex data...");
    GLfloat vertices[] =
    { //        COORDINATES                        |        COLORS        //
        -0.5f, -0.5f * float(sqrt(3)) / 3,    0.0f,    0.8f, 0.3f, 0.02f, //Lower left corner
        0.5f, -0.5f * float(sqrt(3)) / 3,     0.0f,    0.8f, 0.3f, 0.02f, //Lower right corner
        0.0f, 0.5f * float(sqrt(3)) * 2 / 3,  0.0f,    0.0f, 0.6f, 0.32f, //Upper corner
        -0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f,    0.9f, 0.45f, 0.17f, //Inner left
        0.5f / 2, 0.5f * float(sqrt(3)) / 6,  0.0f,    0.9f, 0.45f, 0.17f, //Inner right:
        0.0f, -0.5f * float(sqrt(3)) / 3,     0.0f,    0.8f, 0.3f, 0.02f // Inner down
    };

    GLuint indices[] = // indices are the thing that define the order of vertices to draw
    {
        0, 3, 5,
        3, 2, 4,
        5, 4, 1
    };

    // Create a window
    LOG_INFO("Creating window...");
    GLFWwindow* window = glfwCreateWindow(800, 800, "Basic renderer", nullptr, nullptr);
    if (!window) {
        LOG_FATAL("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    LOG_INFO("Initializing GLEW...");
    glewExperimental = GL_TRUE; // Needed for core profile
    if (GLenum err = glewInit() != GLEW_OK) {
        LOG_FATAL(std::string("Failed to initialize GLEW: ") + (const char*)glewGetErrorString(err));
        glfwTerminate();
        return -1;
    }

    // Print OpenGL version
    LOG_INFO(std::string("OpenGL Version: ") + (const char*)glGetString(GL_VERSION));
    LOG_INFO(std::string("GLEW Version: ") + (const char*)glewGetString(GLEW_VERSION));
    LOG_INFO(std::string("GLFW Version: ") + glfwGetVersionString());

    // Set viewport
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    LOG_DEBUG("Viewport set to " + std::to_string(width) + "x" + std::to_string(height));

    LOG_INFO("Initializing shaders...");
    Shader shader("shaders/default.vert", "shaders/default.frag");

    LOG_INFO("Setting up VAO, VBO, EBO...");
    VAO VAO1;
    VAO1.Bind();

    VBO VBO1(vertices, sizeof(vertices));
    EBO EBO1(indices, sizeof(indices));

    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    GLuint utiID = glGetUniformLocation(shader.ID, "scale");
    LOG_TODO("Make scale a separate setting that is passed from a file");

    // Main loop
    LOG_INFO("Entering main rendering loop");
    while (!glfwWindowShouldClose(window)) {
        // Clear the screen
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        shader.Activate();
        glUniform1f(utiID, 1.0f);//We can do that ONLY after activating our shader program
        VAO1.Bind();
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up
    LOG_INFO("Cleaning up resources...");
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    shader.Delete();

    glfwTerminate();
    LOG_INFO("Application terminated normally");
    return 0;
}
