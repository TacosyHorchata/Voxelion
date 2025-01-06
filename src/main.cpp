#include <iostream>
#include "graphicsEngine/Window.h"
#include "graphicsEngine/Camera.h"
#include "graphicsEngine/ChunkRenderer.h"
#include "core/World.h"
#include "physicsEngine/DebugUtils.h"
#include "core/InputHandler.h"

int main() {
    try {
        Window window(1200, 800, "Voxel Engine");
        World world;
        Camera camera(45.0f, 1200.0f/800.0f);
        ChunkRenderer renderer;
        InputHandler inputHandler;

        // Move camera back to see the world
        camera.position = glm::vec3(8.0f, 8.0f, 20.0f);
        
        // Setup input handling
        inputHandler.setupCallbacks(window.getHandle());
        
        // Enable depth testing
        glEnable(GL_DEPTH_TEST);
        
        // Update mesh with initial world data
        renderer.updateChunkMesh(world);
        
        // Track time for movement
        float lastFrame = 0.0f;
        
        while (!window.shouldClose()) {
            float currentFrame = glfwGetTime();
            float deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            // Process input
            inputHandler.processInput(window.getHandle(), camera, world, deltaTime);

            // Update physics
            world.updatePhysics(deltaTime);
            
            // Update mesh if needed
            if (world.needsRenderUpdate) {
                renderer.updateChunkMesh(world);
                world.needsRenderUpdate = false;
            }

            // Clear screen
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Render world
            renderer.render(camera);

            window.pollEvents();
            window.swapBuffers();
        }
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
} 