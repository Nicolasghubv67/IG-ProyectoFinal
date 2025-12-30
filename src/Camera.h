#pragma once

#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

// Cámara FPS + control de ratón + FOV + tuning (sensibilidad/velocidad)
class CameraFPS {
public:
    // Estado
    glm::vec3 pos   = glm::vec3(0.0f, 5.0f, 8.0f);
    glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up    = glm::vec3(0.0f, 1.0f, 0.0f);

    float yaw   = -90.0f;
    float pitch = 0.0f;

    float fovy             = 60.0f;  // 30..90
    float mouseSensitivity = 0.12f;  // 0.02..0.2
    float speed            = 5.0f;   // 5..25

    bool  mouseCaptured = false;

    // API
    glm::mat4 getView() const {
        return glm::lookAt(pos, pos + front, up);
    }

    // Llamar cada frame: mueve WASD + tuning continuo
    void update(GLFWwindow* window, float dt) {
        processTuning(window, dt);
        processMove(window, dt);
    }

    // Callbacks
    void onScroll(double yoffset) {
        fovy -= static_cast<float>(yoffset) * 2.0f;
        fovy = glm::clamp(fovy, 30.0f, 90.0f);
    }

    void onCursorPos(double xpos, double ypos) {
        if (!mouseCaptured) return;

        if (firstMouse) {
            lastMouseX = xpos;
            lastMouseY = ypos;
            firstMouse = false;
            return;
        }

        double dx = xpos - lastMouseX;
        double dy = lastMouseY - ypos;

        lastMouseX = xpos;
        lastMouseY = ypos;

        yaw   += static_cast<float>(dx) * mouseSensitivity;
        pitch += static_cast<float>(dy) * mouseSensitivity;

        pitch = glm::clamp(pitch, -89.0f, 89.0f);

        glm::vec3 f;
        f.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        f.y = sin(glm::radians(pitch));
        f.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(f);
    }

    void onMouseButton(GLFWwindow* window, int button, int action) {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            captureMouse(window, true);
        }
    }

    // Devuelve true si consume ESC (es decir, si solo suelta ratón)
    bool onEscape(GLFWwindow* window) {
        if (mouseCaptured) {
            captureMouse(window, false);
            return true;
        }
        return false;
    }

    void captureMouse(GLFWwindow* window, bool capture) {
        mouseCaptured = capture;
        firstMouse = true;

        if (capture) {
            glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        } else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }

private:
    // “hold timers” para aceleración
    float holdSensDec  = 0.0f, holdSensInc  = 0.0f;
    float holdSpeedDec = 0.0f, holdSpeedInc = 0.0f;

    bool   firstMouse = true;
    double lastMouseX = 0.0;
    double lastMouseY = 0.0;

    static float accel(float holdSeconds) {
        // 1.0 al inicio, sube progresivo hasta 3x
        float a = 0.5f + 3.0f * (holdSeconds / 1.0f);
        return (a > 3.0f) ? 3.0f : a;
    }

    void processMove(GLFWwindow* window, float dt) {
        float vel = speed * dt;

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) pos += front * vel;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) pos -= front * vel;

        glm::vec3 right = glm::normalize(glm::cross(front, up));
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) pos += right * vel;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) pos -= right * vel;

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)      pos += up * vel;
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) pos -= up * vel;
    }

    void processTuning(GLFWwindow* window, float dt) {
        // Sensibilidad: [ y ]
        bool sensDec = glfwGetKey(window, GLFW_KEY_LEFT_BRACKET)  == GLFW_PRESS;
        bool sensInc = glfwGetKey(window, GLFW_KEY_RIGHT_BRACKET) == GLFW_PRESS;

        holdSensDec = sensDec ? (holdSensDec + dt) : 0.0f;
        holdSensInc = sensInc ? (holdSensInc + dt) : 0.0f;

        float sensStep = 0.20f * dt; // base por segundo
        if (sensDec) mouseSensitivity -= sensStep * accel(holdSensDec);
        if (sensInc) mouseSensitivity += sensStep * accel(holdSensInc);

        mouseSensitivity = glm::clamp(mouseSensitivity, 0.02f, 0.2f);

        // Velocidad: ' y ¡
        bool speedDec = glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS;
        bool speedInc = glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS;

        holdSpeedDec = speedDec ? (holdSpeedDec + dt) : 0.0f;
        holdSpeedInc = speedInc ? (holdSpeedInc + dt) : 0.0f;

        float speedStep = 8.0f * dt;
        if (speedDec) speed -= speedStep * accel(holdSpeedDec);
        if (speedInc) speed += speedStep * accel(holdSpeedInc);

        speed = glm::clamp(speed, 5.0f, 25.0f);
    }
};
