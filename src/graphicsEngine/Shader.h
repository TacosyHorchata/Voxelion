#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
    unsigned int ID;

    Shader(const char* vertexPath, const char* fragmentPath) {
        std::string vertexCode = R"(
            #version 410 core
            layout (location = 0) in vec3 aPos;
            layout (location = 1) in vec3 aColor;
            out vec3 ourColor;
            uniform mat4 projection;
            uniform mat4 view;
            uniform mat4 model;
            void main() {
                gl_Position = projection * view * model * vec4(aPos, 1.0);
                ourColor = aColor;
            }
        )";

        std::string fragmentCode = R"(
            #version 410 core
            in vec3 ourColor;
            out vec4 FragColor;
            void main() {
                FragColor = vec4(ourColor, 1.0);
            }
        )";

        unsigned int vertex, fragment;
        
        // Vertex Shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        const char* vShaderCode = vertexCode.c_str();
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");
        
        // Fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        const char* fShaderCode = fragmentCode.c_str();
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");
        
        // Shader Program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");
        
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    void use() { 
        glUseProgram(ID); 
    }

    void setMat4(const std::string &name, const glm::mat4 &mat) const {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

private:
    void checkCompileErrors(unsigned int shader, std::string type) {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM") {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << std::endl;
            }
        } else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << std::endl;
            }
        }
    }
};