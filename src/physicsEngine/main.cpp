#include "World.h"
#include "Physics.h"
#include "DebugUtils.h"

int main() {
    // Initialize the world and set up some voxels
    World world;
    world.setVoxel(50, 50, 50, true);  // Example: Set a voxel to be solid
    world.setVoxel(50, 50, 51, true);

    while (true) {
        applyGravity(world);    // Apply gravity physics
        printWorld(world);      // Print the world for debugging

        // Additional game or simulation logic here

        // Optional: Add sleep/delay to control the simulation speed
    }

    return 0;
}
