#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../core/World.h"
#include <iostream>

class Camera {
public:
    glm::vec3 position;
    
    Camera(float fov = 45.0f, float aspectRatio = 16.0f/9.0f) 
        : position(0.0f, 0.0f, 3.0f)
        , front(0.0f, 0.0f, -1.0f)
        , up(0.0f, 1.0f, 0.0f)
        , yaw(-90.0f)
        , pitch(0.0f)
        , fov(fov)
        , aspectRatio(aspectRatio) {}

    void moveForward(float deltaTime, const World& world) {
        glm::vec3 newPos = position + front * speed * deltaTime;
        if (!checkCollision(newPos, world)) {
            position = newPos;
        }
    }

    void moveBackward(float deltaTime, const World& world) {
        glm::vec3 newPos = position - front * speed * deltaTime;
        if (!checkCollision(newPos, world)) {
            position = newPos;
        }
    }

    void moveRight(float deltaTime, const World& world) {
        glm::vec3 newPos = position + glm::normalize(glm::cross(front, up)) * speed * deltaTime;
        if (!checkCollision(newPos, world)) {
            position = newPos;
        }
    }

    void moveLeft(float deltaTime, const World& world) {
        glm::vec3 newPos = position - glm::normalize(glm::cross(front, up)) * speed * deltaTime;
        if (!checkCollision(newPos, world)) {
            position = newPos;
        }
    }

    void rotate(float xoffset, float yoffset) {
        yaw += xoffset * sensitivity;
        pitch += yoffset * sensitivity;

        // Constrain pitch
        if(pitch > 89.0f) pitch = 89.0f;
        if(pitch < -89.0f) pitch = -89.0f;

        updateCameraVectors();
    }

    glm::mat4 getViewMatrix() const {
        return glm::lookAt(position, position + front, up);
    }

    glm::mat4 getProjectionMatrix() const {
        return glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 100.0f);
    }

private:
    glm::vec3 front;
    glm::vec3 up;
    
    float yaw;
    float pitch;
    float fov;
    float aspectRatio;
    float speed = 2.5f;
    float sensitivity = 0.1f;

    void updateCameraVectors() {
        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(direction);
    }

    bool checkCollision(const glm::vec3& pos, const World& world) {
        // Check the voxel at camera position
        int x = static_cast<int>(pos.x);
        int y = static_cast<int>(pos.y);
        int z = static_cast<int>(pos.z);

        // Check surrounding voxels (simple collision box)
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                for (int dz = -1; dz <= 1; dz++) {
                    int checkX = x + dx;
                    int checkY = y + dy;
                    int checkZ = z + dz;
                    
                    if (checkX >= 0 && checkX < World::WIDTH &&
                        checkY >= 0 && checkY < World::HEIGHT &&
                        checkZ >= 0 && checkZ < World::DEPTH) {
                        if (world.getVoxel(checkX, checkY, checkZ).solid) {
                            
                            return true;  // Collision detected
                        }
                    }
                }
            }
        }
        return false;  // No collision
    }
};
