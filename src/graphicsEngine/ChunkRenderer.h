#pragma once
#include "../core/Chunk.h"
#include "../core/World.h"
#include "Shader.h"
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>

class ChunkRenderer {
public:
    ChunkRenderer() {
        setupShader();
        setupBuffers();
    }

    void render(const Camera& camera) {
        shader.use();
        
        // Set uniforms
        shader.setMat4("projection", camera.getProjectionMatrix());
        shader.setMat4("view", camera.getViewMatrix());
        shader.setMat4("model", glm::mat4(1.0f));  // Identity matrix for now
        
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
        
        // Check for OpenGL errors
        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR) {
            std::cout << "OpenGL error: " << err << std::endl;
        }
    }

    void updateChunkMesh(const World& world) {
        std::vector<float> vertices;
        std::vector<unsigned int> indices;
        
        // Generate a cube for each solid voxel
        for(int x = 0; x < World::WIDTH; x++) {
            for(int y = 0; y < World::HEIGHT; y++) {
                for(int z = 0; z < World::DEPTH; z++) {
                    if(world.getVoxel(x, y, z).solid) {
                        addCubeToMesh(x, y, z, vertices, indices);
                    }
                }
            }
        }

        // Update GPU buffers
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
        
        indexCount = indices.size();

        std::cout << "Vertex count: " << vertices.size() / 6 << std::endl;
        std::cout << "Index count: " << indices.size() << std::endl;
    }

private:
    GLuint VAO, VBO, EBO;
    Shader shader{"", ""};  // Using embedded shaders from Shader class
    int indexCount = 0;

    void setupShader() {
        shader = Shader("", "");  // Using embedded shaders
    }

    void setupBuffers() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }

    void addCubeToMesh(float x, float y, float z, std::vector<float>& vertices, std::vector<unsigned int>& indices) {
        float size = 0.5f;  // Half-size of the cube
        
        // Generate color based on position
        glm::vec3 color(
            0.5f + (float)x / World::WIDTH * 0.5f,   // Red varies with x
            0.5f + (float)y / World::HEIGHT * 0.5f,  // Green varies with y
            0.5f + (float)z / World::DEPTH * 0.5f    // Blue varies with z
        );
        
        // Vertex positions and colors for a cube
        std::vector<float> cubeVertices = {
            // Front face (position: x,y,z, color: r,g,b)
            x-size, y-size, z+size,  color.r, color.g, color.b,
            x+size, y-size, z+size,  color.r, color.g, color.b,
            x+size, y+size, z+size,  color.r, color.g, color.b,
            x-size, y+size, z+size,  color.r, color.g, color.b,
            
            // Back face
            x-size, y-size, z-size,  color.r, color.g, color.b,
            x+size, y-size, z-size,  color.r, color.g, color.b,
            x+size, y+size, z-size,  color.r, color.g, color.b,
            x-size, y+size, z-size,  color.r, color.g, color.b,
        };
        
        unsigned int startIdx = vertices.size() / 6;
        vertices.insert(vertices.end(), cubeVertices.begin(), cubeVertices.end());
        
        std::vector<unsigned int> cubeIndices = {
            0, 1, 2, 2, 3, 0,  // Front
            1, 5, 6, 6, 2, 1,  // Right
            5, 4, 7, 7, 6, 5,  // Back
            4, 0, 3, 3, 7, 4,  // Left
            3, 2, 6, 6, 7, 3,  // Top
            4, 5, 1, 1, 0, 4   // Bottom
        };
        
        for(unsigned int idx : cubeIndices) {
            indices.push_back(startIdx + idx);
        }
    }
};