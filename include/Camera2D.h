#ifndef CAMERA2D_CLASS_H
#define CAMERA2D_CLASS_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shaderClass.h"

class Camera2D {
public:
    // 2D camera properties
    glm::vec2 position;
    float zoom;
    float rotation; // In radians

    // Viewport dimensions
    int width;
    int height;

    // Camera bounds (for orthographic projection)
    float left, right, bottom, top;

    Camera2D(int width, int height, glm::vec2 position = glm::vec2(0.0f, 0.0f));

    // Update projection and view matrices
    void UpdateMatrices();

    // Send matrices to shader
    void SetMatrices(Shader& shader);

    // Camera controls
    void SetPosition(const glm::vec2& pos) { position = pos; }
    void Translate(const glm::vec2& delta) { position += delta; }
    void SetZoom(float z);
    void SetRotation(float rot) { rotation = rot; }

    // Coordinate conversion
    glm::vec2 ScreenToWorld(const glm::vec2& screenPos);
    glm::vec2 WorldToScreen(const glm::vec2& worldPos);

    // Viewport management
    void SetViewportSize(int w, int h);

private:
    glm::mat3 projectionMatrix;
    glm::mat3 viewMatrix;
    bool matricesDirty;

    void CalculateProjection();
    void CalculateView();
};

#endif
