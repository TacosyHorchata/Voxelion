#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <string>

class Window {
public:
    Window(int width = 800, int height = 600, const std::string& title = "Voxel Engine") 
        : width(width), height(height), title(title) {
        initialize();
    }

    ~Window() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    bool shouldClose() const {
        return glfwWindowShouldClose(window);
    }

    void swapBuffers() {
        glfwSwapBuffers(window);
    }

    void pollEvents() {
        glfwPollEvents();
    }

    GLFWwindow* getHandle() { return window; }

private:
    GLFWwindow* window;
    int width, height;
    std::string title;

    void initialize() {
        if (!glfwInit()) {
            throw std::runtime_error("Failed to initialize GLFW");
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        if (!window) {
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window");
        }

        glfwMakeContextCurrent(window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            throw std::runtime_error("Failed to initialize GLAD");
        }

        glViewport(0, 0, width, height);
        
        // Enable depth testing
        glEnable(GL_DEPTH_TEST);
    }
};