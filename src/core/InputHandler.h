#pragma once
#include <GLFW/glfw3.h>
#include "../graphicsEngine/Camera.h"
#include "../core/World.h"

class InputHandler {
public:
    void processInput(GLFWwindow* window, Camera& camera, const World& world, float deltaTime) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        // Camera movement
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.moveForward(deltaTime, world);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.moveBackward(deltaTime, world);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.moveLeft(deltaTime, world);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.moveRight(deltaTime, world);

        // Mouse handling
        static bool firstMouse = true;
        static double lastX = 0.0, lastY = 0.0;
        
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        if (firstMouse) {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // Reversed since y-coordinates range from bottom to top
        lastX = xpos;
        lastY = ypos;

        camera.rotate(xoffset, yoffset);
    }

    static void setupCallbacks(GLFWwindow* window) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  // Hide and capture cursor
        glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    }

private:
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    }
};