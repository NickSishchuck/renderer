#ifndef IMGUI_MANAGER_H
#define IMGUI_MANAGER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

class ImGuiManager {
private:
    GLFWwindow* window;

public:
    ImGuiManager(GLFWwindow* window);
    ~ImGuiManager();

    void Initialize();
    void BeginFrame();
    void EndFrame();
    void Render();
    void Shutdown();
};

#endif
