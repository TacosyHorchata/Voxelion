#include "Physics.h"
#include <utility>
#include <cmath>

bool isVoxelSolid(int x, int y, int z, World &world) {
    return world.getVoxel(x, y, z).solid;
}

//movement
void applyMovement(World &world) {
    for (int x = 0; x < World::WIDTH; ++x) {
        for (int y = 0; y < World::HEIGHT; ++y) {
            for (int z = 0; z < World::DEPTH; ++z) {
                Voxel& voxel = world.getVoxel(x, y, z);
                if (!voxel.solid || !voxel.affectedByGravity) continue;

                // Calculate new position based on velocity
                int newX = x + static_cast<int>(voxel.velocityX);
                int newY = y + static_cast<int>(voxel.velocityY);
                int newZ = z + static_cast<int>(voxel.velocityZ);

                // Check bounds
                if (newX >= 0 && newX < World::WIDTH &&
                    newY >= 0 && newY < World::HEIGHT &&
                    newZ >= 0 && newZ < World::DEPTH) {
                    
                    // Check if destination is empty
                    if (!isVoxelSolid(newX, newY, newZ, world)) {
                        // Move voxel to new position
                        std::swap(world.world[x][y][z], world.world[newX][newY][newZ]);
                    } else {
                        // Handle collision
                        Voxel& targetVoxel = world.getVoxel(newX, newY, newZ);
                        float relativeVelocity = std::sqrt(
                            std::pow(voxel.velocityX - targetVoxel.velocityX, 2) +
                            std::pow(voxel.velocityY - targetVoxel.velocityY, 2) +
                            std::pow(voxel.velocityZ - targetVoxel.velocityZ, 2)
                        );
                        handleCollision(voxel, targetVoxel, relativeVelocity);
                    }
                }
            }
        }
    }
}

void applyGravity(World &world) {
    bool changes = false;
    for (int x = 0; x < World::WIDTH; ++x) {
        for (int y = 0; y < World::HEIGHT; ++y) {
            for (int z = World::DEPTH - 2; z >= 0; --z) {
                if (world.getVoxel(x, y, z).solid && !isVoxelSolid(x, y, z + 1, world)) {
                    std::swap(world.world[x][y][z], world.world[x][y][z + 1]);
                    changes = true;
                }
            }
        }
    }
    if (changes) {
        applyGravity(world);
    }
}

void handleCollision(Voxel& v1, Voxel& v2, float impactVelocity) {
    float impactForce = calculateImpactForce(v1, v2, impactVelocity);
    
    // Apply damage based on impact force
    updateDurability(v1, impactForce);
    updateDurability(v2, impactForce);
}

void calculateCollisionForce(Voxel& v1, Voxel& v2, float impactVelocity) {
    // Calculate collision force...
}

float calculateImpactForce(const Voxel& v1, const Voxel& v2, float relativeVelocity) {
    // Simple impact force calculation
    float combinedMass = v1.density + v2.density;
    return combinedMass * relativeVelocity * relativeVelocity;
}

void updateDurability(Voxel& voxel, float impactForce) {
    // Reduce durability based on impact force
    float damage = impactForce * 0.1f;  // Scale down the impact force
    if (damage > 0) {
        int durabilityLoss = static_cast<int>(damage);
        voxel.durability = (durabilityLoss >= voxel.durability) ? 0 : voxel.durability - durabilityLoss;
    }
}

// basic interactions
// 0. movement
// 1. gravity
// 2. collision
// 3. fluid dynamics
// 4. attraction/repulsion
// 5. destruction