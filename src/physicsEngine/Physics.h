#pragma once
#include "../core/World.h"
#include "../core/Voxel.h"

// Function declarations
void applyGravity(World &world);
bool isVoxelSolid(int x, int y, int z, World &world);

// Temperature-related functions
void updateTemperature(World& world, float deltaTime);
bool checkStateChange(Voxel& voxel);

// Force and movement
void applyForce(World& world, int x, int y, int z, float forceX, float forceY, float forceZ);
float calculateImpactForce(const Voxel& v1, const Voxel& v2, float relativeVelocity);

// Material interactions
void handleCollision(Voxel& v1, Voxel& v2, float impactVelocity);
void updateDurability(Voxel& voxel, float impactForce);

// Fluid dynamics
void simulateFluid(World& world);
float calculatePressure(const Voxel& voxel, float depth);
