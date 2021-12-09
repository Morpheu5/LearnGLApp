#ifndef LearnGLApp_hpp
#define LearnGLApp_hpp

#include <vector>
#include <memory>

#include "BaseApp.hpp"
#include "Shader.hpp"

class LearnGLApp : public BaseApp {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    unsigned int VBO;
    unsigned int VAO;
    unsigned int EBO;

    std::shared_ptr<Shader> shaderProgram;

public:
    virtual void setup();
    virtual void run();

    virtual void resize(int width, int height) {
        glViewport(0, 0, width, height);
    }

    virtual void processInput();

    unsigned int createShaderFromFile(unsigned int shaderType, const char* const filename) {
        // Load the shader code from file
        auto shaderCode = readFile(filename);
        
        // Create the shader object
        unsigned int shaderObject = glCreateShader(shaderType);
        // Attach the shader code to the shader object
        glShaderSource(shaderObject, 1, &shaderCode, nullptr);
        // Compile the shader
        glCompileShader(shaderObject);
        // Check if the compilation was successful
        int success;
        char infoLog[512];
        glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shaderObject, 512, nullptr, infoLog);
            std::string message = "Vertex shader compilation failed\n";
            message.append(infoLog);
            throw std::runtime_error(message);
        }
        // Return the shader's handler
        return shaderObject;
    }

    unsigned int createShaderProgram(std::vector<unsigned int> shaders) {
        unsigned int program = glCreateProgram();
        for (unsigned int i = 0; i < shaders.size(); ++i) {
            auto shader = shaders[i];
            glAttachShader(program, shader);
        }
        glLinkProgram(program);
        // Check that the shader program was created correctly
        int success;
        char infoLog[512];
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(program, 512, nullptr, infoLog);
            std::string message = "Failed to create a shader program\n";
            message.append(infoLog);
            throw std::runtime_error(message);
        }
        // Return the program's handler
        return program;
    }

    virtual ~LearnGLApp();
};

#endif
