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
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Let's make a square
    vertices = {
        0.0f,  0.5f, 0.0f,  // top center
        0.5f, -0.5f, 0.0f,  // bottom right
       -0.5f, -0.5f, 0.0f,  // bottom left
    };
    indices = {
        0, 1, 2,
    };

    // Create vertex array object
    glGenVertexArrays(1, &VAO);
    // Bind vertex array object
    glBindVertexArray(VAO);

    // Create vertex buffer object
    glGenBuffers(1, &VBO);
    // Bind vertex buffer object
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Set data for the vertex buffer object
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    // Create the element buffer object
    glGenBuffers(1, &EBO);
    // Bind the element buffer object
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // Set the data for the element buffer object
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

    // Describe the attribute layout of the data
    // location, size (items), type, normalize, stride (bytes), offset (bytes)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

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
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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
