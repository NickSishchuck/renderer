#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../include/stb_image.h"
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
    { //        COORDINATES                   |    COLORS       |     COORDINATES
        -0.5f, -0.5f, 0.0f,                   1.0f, 0.0f, 0.0f,     0.0f, 0.0f,  //Lower left
        -0.5f, 0.5f, 0.0f,                    0.0f, 1.0f, 0.0f,     0.0f, 1.0f,  //Upper left
        0.5f, 0.5f, 0.0f,                     0.0f, 0.0f, 1.0f,     1.0f, 1.0f,  //Upper right
        0.5f, -0.5f, 0.0f,                    1.0f, 1.0f, 1.0f,     1.0f, 0.0f   //Lower right
    };

    GLuint indices[] = // indices are the thing that define the order of vertices to draw
    {
        0,2,1, //Upper triangle
        0,3,2 //Lower triangle
    };

    LOG_INFO("Initializing shaders...");
    Shader shader("shaders/default.vert", "shaders/default.frag");

    LOG_INFO("Setting up VAO, VBO, EBO...");
    VAO VAO1;
    VAO1.Bind();

    VBO VBO1(vertices, sizeof(vertices));
    EBO EBO1(indices, sizeof(indices));

    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    GLuint utiID = glGetUniformLocation(shader.ID, "scale");

    // Texture
    int widthImage, heightImage, numColorChannels;
    unsigned char* bytes = stbi_load("textures/pop_cat.jpg", &widthImage, &heightImage, &numColorChannels, 0);

    if (bytes = NULL){
        LOG_ERROR("Didn't load the texture");
    } else {
        LOG_INFO("Sucsessfully loaded the texture");
    }
    GLuint texture;
    glGenTextures(1, &texture);
    LOG_GLERROR("Failed to generate texture");
    // Check for OpenGL errors after generating texture
    GLenum err = glGetError();

    //Texture units are slots for textures. In each slot could be up to 16 textures

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    LOG_GLERROR("Failed to bind texture");

    //First setting is what to choose if the image is scaled up?
    //Nearest is pixel art
    //Linear is blurry
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


    //Second setting is what to choose if the image is repeated
    //GL_REPEAT
    //GL_MIRRORED_REPEAT
    //GL_CLAMP_TO_EDGE
    //GL_CLAMP_TO_BORDER
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //If we want to use clamp to border:
    // float flatcolor[] = {R, G, B};
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatcolor);

    //Generating our texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImage, heightImage, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);
    //GL_RGB = For .jpeg
    //GL_RGBA = For .png
    LOG_GLERROR("Failed to load texture data");

    glGenerateMipmap(GL_TEXTURE_2D); //Mipmap - smaller resolution versions of tha same texture, that are used when the texture is far away (for example)

    //deleting, unbinding
    stbi_image_free(bytes);
    glBindTexture(GL_TEXTURE_2D,0);

    GLuint tex0Uniform = glGetUniformLocation(shader.ID, "tex0");
    shader.Activate();
    glUniform1i(tex0Uniform, 0);



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
        glBindTexture(GL_TEXTURE_2D, texture);


        VAO1.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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
    glDeleteTextures(1,&texture);

    // Shut down ImGui
    imguiManager.Shutdown();

    glfwTerminate();
    LOG_INFO("Application terminated normally");
    return 0;
}
