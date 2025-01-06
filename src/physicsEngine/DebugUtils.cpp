#include "DebugUtils.h"
#include <iostream>

void printWorld(World &world) {
    for (int z = 0; z < World::DEPTH; ++z) {
        for (int y = 0; y < World::HEIGHT; ++y) {
            for (int x = 0; x < World::WIDTH; ++x) {
                std::cout << (world.getVoxel(x, y, z).solid ? "#" : ".");
            }
            std::cout << std::endl;
        }
        std::cout << "------------" << std::endl;
    }
}
