#pragma once
#include "../graphicsEngine/Window.h"
#include "../graphicsEngine/Camera.h"
#include "../graphicsEngine/ChunkRenderer.h"
#include "World.h"
#include "InputHandler.h"

class Game {
public:
    Game() : window(1280, 720, "Voxelion") {
        initialize();
    }

    void run() {
        while (!window.shouldClose()) {
            float currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            update();
            render();

            window.swapBuffers();
            window.pollEvents();
        }
    }

private:
    Window window;
    Camera camera;
    World world;
    ChunkRenderer chunkRenderer;
    InputHandler inputHandler;
    
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    void initialize() {
        // Setup initial game state
    }

    void update() {
        // Update game logic
        world.updatePhysics(deltaTime);
        // Update camera based on input
        inputHandler.processInput(window.getHandle(), camera, deltaTime);
    }

    void render() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        auto viewProj = camera.getProjectionMatrix() * camera.getViewMatrix();
        chunkRenderer.render(viewProj);
    }
};
