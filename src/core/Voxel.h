#pragma once
#include <cstdint>
#include <glm/glm.hpp>

struct Voxel {
    // Position
    float positionX = 0.0f;
    float positionY = 0.0f;
    float positionZ = 0.0f;
    
    // Velocity
    float velocityX = 0.0f;
    float velocityY = 0.0f;
    float velocityZ = 0.0f;

    // Properties
    bool solid = false;           // True if voxel is solid, false if it's empty
    bool affectedByGravity = false; // Whether gravity affects this voxel
    bool isStatic = true;         // Whether the voxel can move
    float density = 1.0f;         // Material density (kg/m³)
    uint8_t durability = 100;     // How much damage the voxel can take before breaking (0-255)
    uint8_t state = 0;           // Material state (0=solid, 1=liquid, 2=gas)
    
    Voxel() = default;

    Voxel(bool solid, bool gravity = false) 
        : solid(solid)
        , affectedByGravity(gravity)
        , isStatic(!gravity) {}
};
