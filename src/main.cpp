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
#include "../include/ImGuiManager.h"

// Error callback for GLFW
void errorCallback(int error, const char* description) {
    LOG_ERROR(std::string("GLFW Error ") + std::to_string(error) + ": " + description);
}

// Handle window resize
void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {
    // Initialize Logger first
    Logger* logger = Logger::getInstance();
    logger->enableColors(true);

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

    // Create a window
    LOG_INFO("Creating window...");
    GLFWwindow* window = glfwCreateWindow(1300, 900, "ImGui OpenGL Renderer", nullptr, nullptr);
    if (!window) {
        LOG_FATAL("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

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

    // Setup ImGui
    ImGuiManager imguiManager(window);
    imguiManager.Initialize();

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

    // State variables
    float scale = 1.0f;
    float clearColor[4] = {0.2f, 0.3f, 0.3f, 1.0f};
    bool showDemoWindow = false;

    // Main loop
    LOG_INFO("Entering main rendering loop");
    while (!glfwWindowShouldClose(window)) {
        // Poll events first
        glfwPollEvents();

        // 1. Render the scene to the backbuffer
        glClearColor(clearColor[0], clearColor[1], clearColor[2], 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Make sure we're using the full window for rendering
        int windowWidth, windowHeight;
        glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
        glViewport(0, 0, windowWidth, windowHeight);

        // Render the triangle directly to the backbuffer
        shader.Activate();
        glUniform1f(utiID, scale);
        VAO1.Bind();
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

        // 2. Now render ImGui on top
        imguiManager.BeginFrame();

        // Create a control window (floating over the scene)
        {
            // Position the controls in the top-left corner
            ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
            ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiCond_FirstUseEver);

            ImGui::Begin("Controls");

            ImGui::Text("Renderer Settings");
            ImGui::SliderFloat("Scale", &scale, 0.1f, 2.0f);
            ImGui::ColorEdit3("Background", clearColor);
            ImGui::Checkbox("Show ImGui Demo Window", &showDemoWindow);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                        1000.0f / ImGui::GetIO().Framerate,
                        ImGui::GetIO().Framerate);

            ImGui::End();

            // Show the demo window if enabled
            if (showDemoWindow) {
                ImGui::ShowDemoWindow(&showDemoWindow);
            }
        }

        // End ImGui frame and render it
        imguiManager.EndFrame();
        imguiManager.Render();

        // Swap buffers
        glfwSwapBuffers(window);
    }

    // Clean up
    LOG_INFO("Cleaning up resources...");
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    shader.Delete();

    // Shut down ImGui
    imguiManager.Shutdown();

    glfwTerminate();
    LOG_INFO("Application terminated normally");
    return 0;
}
