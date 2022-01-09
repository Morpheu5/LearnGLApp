#ifndef LearnGLApp_hpp
#define LearnGLApp_hpp

#include <vector>
#include <memory>

#include <glm/glm.hpp>

#include "BaseApp.hpp"
#include "Shader.hpp"
#include "Camera.hpp"

class LearnGLApp : public BaseApp {
    glm::vec2 viewportSize = glm::vec2();
    glm::vec2 lastMousePos = glm::vec2();

    bool firstMouseEvent = true;
    Camera camera = Camera(glm::vec3(-2.0f, 2.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, -20.0f);

    std::vector<float> vertices;

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    unsigned int VBO;
    unsigned int cubeVAO;
    unsigned int lightVAO;

    // GLuint uvgrid;
    // GLuint white_bear;
    GLuint diffuseMap;

    std::shared_ptr<Shader> phongShader;
    std::shared_ptr<Shader> lampShader;

    glm::vec3 lightPosition = { 1.2f, 1.0f, 2.0f };

public:
    void extracted();
    
    virtual void setup();
    virtual void run();

    virtual void resize(int width, int height) override {
        glViewport(0, 0, width, height);
        viewportSize.x = width;
        viewportSize.y = height;
    }

    virtual void mouseMoved(float xPos, float yPos) override;
    virtual void mouseScrolled(float xOffset, float yOffset) override;

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
