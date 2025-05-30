#include "../include/Camera.h"
#include <GLFW/glfw3.h>
#include <cstdlib>




Camera::Camera(int width, int height, glm::vec3 position){
    Camera::width = width;
    Camera::height = height;
    Position = position;
}

void Camera::Matrix(float FOVdeg, float nearPlane, float farPlane, Shader& shader, const char* uniform){
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    view = glm::lookAt(Position, Position + Orientation, Up);

    // Fix: Cast to float before division to get correct aspect ratio
    projection = glm::perspective(glm::radians(FOVdeg), (float)width / (float)height, nearPlane, farPlane);

    glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(projection * view));
}

//Input controls

void Camera::Inputs(GLFWwindow* window){
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        Position += speed * Orientation;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        Position += speed * -glm::normalize(glm::cross(Orientation, Up));
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        Position += speed * -Orientation;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        Position += speed * glm::normalize(glm::cross(Orientation, Up));
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS){
        Position += speed * Up;
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS){
        Position += speed * -Up;
    }


    //Mouse controls
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS){
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        if (firstClick == true){

            glfwSetCursorPos(window, (width / 2), (height / 2));
            firstClick = false;
        }
        double mouseX;
        double mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);



        float rotx = sensitivity * (float)(mouseY-(height / 2)) / height;
        float roty = sensitivity * (float)(mouseX-(width / 2)) / height;

        glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotx), glm::normalize(glm::cross(Orientation, Up)));

        if (!((glm::angle(newOrientation, Up) <= glm::radians(5.0)) or glm::angle(newOrientation, -Up) <= glm::radians(5.0f))){
            Orientation = newOrientation;
        }

        Orientation = glm::rotate(Orientation, glm::radians(-roty), Up);
        glfwSetCursorPos(window, (width / 2), (height / 2));

    } else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) {
       glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
       firstClick == true;
    }
}
