cmake_minimum_required(VERSION 3.10)
project(VoxelEngine)

set(CMAKE_CXX_STANDARD 17)

# Add GLAD
add_library(glad STATIC ${PROJECT_SOURCE_DIR}/external/glad/src/glad.c)
target_include_directories(glad PUBLIC ${PROJECT_SOURCE_DIR}/external/glad/include)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

# Include directories
include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/external/glad/include
    ${PROJECT_SOURCE_DIR}/external/glm
)

# Define source files
set(SOURCES 
    src/main.cpp
    src/core/World.cpp
    src/core/Voxel.cpp
    src/physicsEngine/Physics.cpp
    src/physicsEngine/DebugUtils.cpp
)

# Define the executable target
add_executable(VoxelEngine ${SOURCES})

# Link libraries
target_link_libraries(VoxelEngine 
    glad
    glfw 
    OpenGL::GL
)