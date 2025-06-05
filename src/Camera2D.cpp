#include "../include/Camera2D.h"
#include <algorithm>

Camera2D::Camera2D(int width, int height, glm::vec2 pos)
    : position(pos), zoom(1.0f), rotation(0.0f), width(width), height(height),
      matricesDirty(true) {
    SetViewportSize(width, height);
}

void Camera2D::SetViewportSize(int w, int h) {
    width = w;
    height = h;

    // Set orthographic bounds based on viewport
    float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
    float halfHeight = static_cast<float>(height) / 2.0f;
    float halfWidth = halfHeight * aspectRatio;

    left = -halfWidth;
    right = halfWidth;
    bottom = -halfHeight;
    top = halfHeight;

    matricesDirty = true;
}

void Camera2D::SetZoom(float z) {
    zoom = std::max(0.1f, z); // Prevent negative or zero zoom
    matricesDirty = true;
}

void Camera2D::UpdateMatrices() {
    if (matricesDirty) {
        CalculateProjection();
        CalculateView();
        matricesDirty = false;
    }
}

void Camera2D::CalculateProjection() {
    // 2D orthographic projection matrix (3x3)
    float zoomedLeft = left / zoom;
    float zoomedRight = right / zoom;
    float zoomedBottom = bottom / zoom;
    float zoomedTop = top / zoom;

    projectionMatrix = glm::mat3(
        2.0f / (zoomedRight - zoomedLeft), 0.0f, -(zoomedRight + zoomedLeft) / (zoomedRight - zoomedLeft),
        0.0f, 2.0f / (zoomedTop - zoomedBottom), -(zoomedTop + zoomedBottom) / (zoomedTop - zoomedBottom),
        0.0f, 0.0f, 1.0f
    );
}

void Camera2D::CalculateView() {
    // 2D view matrix (translation and rotation)
    glm::mat3 translation = glm::mat3(
        1.0f, 0.0f, -position.x,
        0.0f, 1.0f, -position.y,
        0.0f, 0.0f, 1.0f
    );

    if (rotation != 0.0f) {
        float cos_r = cos(rotation);
        float sin_r = sin(rotation);

        glm::mat3 rotationMat = glm::mat3(
            cos_r, -sin_r, 0.0f,
            sin_r, cos_r, 0.0f,
            0.0f, 0.0f, 1.0f
        );

        viewMatrix = rotationMat * translation;
    } else {
        viewMatrix = translation;
    }
}

void Camera2D::SetMatrices(Shader& shader) {
    glm::mat4 projection = glm::ortho(
        -200.0f, 200.0f,  // left, right (X range)
        -250.0f, 400.0f,  // bottom, top (Y range)
        -1.0f, 1.0f       // near, far (Z range for 2D)
    );

    glm::mat4 view = glm::mat4(1.0f); // Identity for 2D

    // Set uniforms
    GLuint projLoc = glGetUniformLocation(shader.ID, "projection2D");
    GLuint viewLoc = glGetUniformLocation(shader.ID, "view2D");

    if (projLoc != -1) {
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
    }
    if (viewLoc != -1) {
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    }

    std::cout << "DEBUG: 2D Camera projection set to cover X:(-200,200) Y:(-250,400)" << std::endl;
}


glm::vec2 Camera2D::ScreenToWorld(const glm::vec2& screenPos) {
    // Convert screen coordinates to world coordinates
    float x = (screenPos.x / width) * (right - left) / zoom + left / zoom + position.x;
    float y = ((height - screenPos.y) / height) * (top - bottom) / zoom + bottom / zoom + position.y;
    return glm::vec2(x, y);
}

glm::vec2 Camera2D::WorldToScreen(const glm::vec2& worldPos) {
    // Convert world coordinates to screen coordinates
    float x = ((worldPos.x - position.x) * zoom - left / zoom) / (right - left) * width;
    float y = height - ((worldPos.y - position.y) * zoom - bottom / zoom) / (top - bottom) * height;
    return glm::vec2(x, y);
}
