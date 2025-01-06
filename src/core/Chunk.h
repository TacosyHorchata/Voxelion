#pragma once
#include "Voxel.h"
#include <array>
#include <memory>
#include <glm/glm.hpp>  // For vertex data
#include <stdexcept>  // Add this for std::out_of_range

class Chunk {
public:
    static const int CHUNK_SIZE = 16;
    
    Chunk(int chunkX, int chunkY, int chunkZ) 
        : chunkX(chunkX), chunkY(chunkY), chunkZ(chunkZ) {
        // Initialize voxels
        for (int i = 0; i < CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE; i++) {
            voxels[i] = Voxel();
        }
    }

    // Physics-related methods
    Voxel& getVoxel(int x, int y, int z) {
        if (!isInBounds(x, y, z)) {
            throw std::out_of_range("Voxel position out of bounds");
        }
        return voxels[getIndex(x, y, z)];
    }

    void setVoxel(int x, int y, int z, const Voxel& voxel) {
        if (isInBounds(x, y, z)) {
            voxels[getIndex(x, y, z)] = voxel;
            isDirty = true;  // Mark for render update
        }
    }

    // Graphics-related methods
    bool needsRenderUpdate() const { return isDirty; }
    void markRenderUpdated() { isDirty = false; }

    // Utility methods
    bool isInBounds(int x, int y, int z) const {
        return x >= 0 && x < CHUNK_SIZE &&
               y >= 0 && y < CHUNK_SIZE &&
               z >= 0 && z < CHUNK_SIZE;
    }

    glm::ivec3 getWorldPosition() const {
        return glm::ivec3(
            chunkX * CHUNK_SIZE,
            chunkY * CHUNK_SIZE,
            chunkZ * CHUNK_SIZE
        );
    }

private:
    std::array<Voxel, CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE> voxels;
    int chunkX, chunkY, chunkZ;
    bool isDirty = true;  // For render optimization

    int getIndex(int x, int y, int z) const {
        return x + CHUNK_SIZE * (y + CHUNK_SIZE * z);
    }
}; 