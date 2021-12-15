#include "LearnGLApp.hpp"
#include "Shader.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <stdexcept>
#include <cmath>

void LearnGLApp::setup() {
    // Create the shader program
    shaderProgram = std::make_shared<Shader>("resources/shaders/shader.vert", "resources/shaders/shader.frag");

    // Let's make a square
    vertices = {
        // positions          // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 1.0f,   // top right
         0.5f, -0.5f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   0.0f, 1.0f,   // top left
    };
    indices = {
        0, 1, 3,
        1, 2, 3,
    };

    // Create vertex array object
    glGenVertexArrays(1, &VAO);
    // Create vertex buffer object
    glGenBuffers(1, &VBO);
    // Create the element buffer object
    glGenBuffers(1, &EBO);

    // Bind vertex array object
    glBindVertexArray(VAO);
    // Bind vertex buffer object
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Set data for the vertex buffer object
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(float) * vertices.size()), vertices.data(), GL_STATIC_DRAW);

    // Bind the element buffer object
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // Set the data for the element buffer object
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(unsigned int) * indices.size()), indices.data(), GL_STATIC_DRAW);

    // Describe the attribute layout of the data
    // location, size (items), type, normalize, stride (bytes), offset (bytes)
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), static_cast<const void *>(nullptr));
    glEnableVertexAttribArray(0);
    // texture coordinates (uv)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<const void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Load the image data for the texture
    uvgrid = loadTexture("resources/textures/uvgrid.png");
    white_bear = loadTexture("resources/textures/white_bear.png");

    shaderProgram->use();
    // Use the locations, not the texture IDs.
    shaderProgram->setInt("uvgrid", 0);
    shaderProgram->setInt("white_bear", 1);

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

        glm::mat4 transform(1.0f);
        transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
        transform = glm::rotate(transform, static_cast<float>(glfwGetTime()), glm::vec3(0.0, 0.0, 1.0));

        // Activate the shader
        shaderProgram->use();
        shaderProgram->setMat4f("transform", transform);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, uvgrid);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, white_bear);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

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
