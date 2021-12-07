#include "LearnGLApp.hpp"
#include <iostream>
#include <stdexcept>

void LearnGLApp::setup() {
    // Compile the shaders
    vertexShader = createShaderFromFile(GL_VERTEX_SHADER, "resources/shaders/shader.vert");
    fragmentShader = createShaderFromFile(GL_FRAGMENT_SHADER, "resources/shaders/shader.frag");
    // Create shader program
    shaderProgram = createShaderProgram({ vertexShader, fragmentShader });
    // Use shader program
    glUseProgram(shaderProgram);
    // Clean up the shaders
//    glDeleteShader(vertexShader);
//    glDeleteShader(fragmentShader);

    // Let's make a triangle
    vertices = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    // Create vertex array object
    glGenVertexArrays(1, &VAO);
    // Create vertex buffer object
    glGenBuffers(1, &triangleVBO);
    // Bind vertex array object
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
    glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    // location, size (items), type, normalize, stride (bytes), offset (bytes)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // The VBO is already registered so we can safely unbind here
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Unbind the VAO so it doesn't get accidentally modified
    glBindVertexArray(0);

    // This one renders in wireframe
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void LearnGLApp::run() {
    while(!glfwWindowShouldClose(_window)) {
        processInput();
        
        // Render stuff
        glClearColor(0.2f, 0.3, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 4);

        // Do the framebuffer swappy thing
        glfwSwapBuffers(_window);

        // Handle events
        glfwPollEvents();
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
