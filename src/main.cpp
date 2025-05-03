#include <cmath>
#include <cstdlib>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../include/shaderClass.h"

#include "../include/VAO.h"
#include "../include/VBO.h"
#include "../include/EBO.h"


// Error callback for GLFW
void errorCallback(int error, const char* description) {
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }



    glfwSetErrorCallback(errorCallback);

    // Set OpenGL version and profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //Triangle
    GLfloat vertices[] =
    {
        -0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, //Lower left corner
        0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, //Lower right corner
        0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f, //Upper corner
        -0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, //Inner left
        0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, //Inner right:
        0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f // Inner down
    };

    GLuint indices[] = // indices are the thing that define the order of vertices to draw
    {
        0, 3, 5,
        3, 2, 4,
        5, 4, 1
    };



    // Create a window
    GLFWwindow* window = glfwCreateWindow(800, 800, "Basic renderer", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = GL_TRUE; // Needed for core profile
    if (GLenum err = glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(err) << std::endl;
        glfwTerminate();
        return -1;
    }

    // Print OpenGL version
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLEW Version: " << glewGetString(GLEW_VERSION) << std::endl;
    std::cout << "GLFW Version: " << glfwGetVersionString() << std::endl;

    // Set viewport
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);


 Shader shader("shaders/default.vert", "shaders/default.frag");

 VAO VAO1;
 VAO1.Bind();

 VBO VBO1(vertices, sizeof(vertices));
 EBO EBO1(indices, sizeof(indices));

 VAO1.LinkVBO(VBO1, 0);
 VAO1.Unbind();
 VBO1.Unbind();
 EBO1.Unbind();
    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Clear the screen
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        shader.Activate();
        VAO1.Bind();
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    // Clean up
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    shader.Delete();

    glfwTerminate();
    return 0;
}
