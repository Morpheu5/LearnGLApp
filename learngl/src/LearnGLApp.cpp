#include "LearnGLApp.hpp"
#include <iostream>
#include <stdexcept>

void LearnGLApp::setup() {
    // Let's make a triangle
    vertices = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    glGenBuffers(1, &triangleVBO);
    glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size(), vertices.data(), GL_STATIC_DRAW);

    auto vertexShaderCode = readFile("resources/shaders/shader.vert");
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderCode, nullptr);
}

void LearnGLApp::run() {
    while(!glfwWindowShouldClose(_window)) {
        processInput();
        
        // Render stuff
        glClearColor(0.2f, 0.3, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Handle events
        glfwPollEvents();

        // Do the framebuffer swappy thing
        glfwSwapBuffers(_window);
    }
}

void LearnGLApp::processInput() {
    if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(_window, GLFW_TRUE);
    }
}


LearnGLApp::~LearnGLApp() {
    glfwTerminate();
}
