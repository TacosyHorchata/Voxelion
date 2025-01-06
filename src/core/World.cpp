#include "World.h"
#include <iostream>

void World::updatePhysics(float deltaTime) {
    const float GRAVITY = -5.0f;
    const float MAX_VELOCITY = 3.0f;
    bool worldChanged = false;  // Track if any voxel moved
    
    for (int x = 0; x < WIDTH; x++) {
        for (int y = HEIGHT-1; y >= 0; y--) {
            for (int z = 0; z < DEPTH; z++) {
                Voxel& voxel = getVoxel(x, y, z);
                
                if (!voxel.solid || voxel.isStatic) continue;

                // Apply gravity with velocity limit
                if (voxel.affectedByGravity) {
                    voxel.velocityY += GRAVITY * deltaTime;
                    voxel.velocityY = std::max(voxel.velocityY, -MAX_VELOCITY);
                }

                // Calculate new position
                float newPosY = voxel.positionY + voxel.velocityY * deltaTime;
                
                // Check for collisions
                int currentY = static_cast<int>(voxel.positionY);
                int newY = static_cast<int>(newPosY);

                // Only move one cell at a time
                if (abs(newY - currentY) > 1) {
                    newY = currentY + (newY > currentY ? 1 : -1);
                    newPosY = static_cast<float>(newY);
                    voxel.velocityY *= 0.5f;
                }

                // Check collision with floor and voxels below
                bool collision = newY <= 0 || 
                               (newY < HEIGHT && getVoxel(x, newY-1, z).solid);

                if (collision) {
                    voxel.velocityY = 0.0f;
                    voxel.isStatic = true;
                    worldChanged = true;
                } else {
                    voxel.positionY = newPosY;
                    
                    // Move voxel to new cell if boundary crossed
                    if (newY != y && newY >= 0 && newY < HEIGHT) {
                        setVoxel(x, newY, z, voxel);
                        setVoxel(x, y, z, Voxel());
                        worldChanged = true;
                    }
                }
            }
        }
    }
    
    if (worldChanged) {
        needsRenderUpdate = true;
    }
}
