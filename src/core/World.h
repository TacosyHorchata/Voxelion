#pragma once
#include "Voxel.h"
#include "Chunk.h"
#include <stdexcept>
#include <random>
#include <glm/glm.hpp>

class World {
public:
    static const int WIDTH = 16;
    static const int HEIGHT = 16;
    static const int DEPTH = 16;

    World() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> height_dist(1, HEIGHT/2);
        std::uniform_real_distribution<> chance(0, 1);

        // Create a solid floor
        for (int x = 0; x < WIDTH; x++) {
            for (int z = 0; z < DEPTH; z++) {
                setVoxel(x, 0, z, true);
            }
        }

        // Add falling voxels at the top
        for (int x = 0; x < WIDTH; x++) {
            for (int z = 0; z < DEPTH; z++) {
                if (chance(gen) < 0.3) {  // 30% chance of falling voxel
                    int y = HEIGHT - 1;  // Start at top
                    Voxel fallingVoxel(true, true);  // solid and affected by gravity
                    fallingVoxel.positionX = x;
                    fallingVoxel.positionY = y;
                    fallingVoxel.positionZ = z;
                    setVoxel(x, y, z, fallingVoxel);
                }
            }
        }
    }

    // Set voxel with full properties
    void setVoxel(int x, int y, int z, const Voxel& voxel) {
        if (isInBounds(x, y, z)) {
            world[x][y][z] = voxel;
        } else {
            throw std::out_of_range("Voxel position out of bounds");
        }
    }

    // Simplified setter for just toggling solid state
    void setVoxel(int x, int y, int z, bool solid) {
        if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT && z >= 0 && z < DEPTH) {
            world[x][y][z].solid = solid;
        }
    }

    Voxel& getVoxel(int x, int y, int z) {
        if (isInBounds(x, y, z)) {
            return world[x][y][z];
        }
        throw std::out_of_range("Voxel position out of bounds");
    }

    const Voxel& getVoxel(int x, int y, int z) const {
        if (isInBounds(x, y, z)) {
            return world[x][y][z];
        }
        throw std::out_of_range("Voxel position out of bounds");
    }

    // Just declare the function
    void updatePhysics(float deltaTime);

    friend void applyMovement(World& world);
    friend void applyGravity(World& world);

    void swapVoxels(int x1, int y1, int z1, int x2, int y2, int z2) {
        if (isValidPosition(x1, y1, z1) && isValidPosition(x2, y2, z2)) {
            std::swap(world[x1][y1][z1], world[x2][y2][z2]);
        }
    }

    bool needsRenderUpdate = false;

private:
    Voxel world[WIDTH][HEIGHT][DEPTH];

    bool isInBounds(int x, int y, int z) const {
        return x >= 0 && x < WIDTH &&
               y >= 0 && y < HEIGHT &&
               z >= 0 && z < DEPTH;
    }

    bool isValidPosition(int x, int y, int z) const {
        return x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT && z >= 0 && z < DEPTH;
    }
};
