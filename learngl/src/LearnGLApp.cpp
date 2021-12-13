#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "LearnGLApp.hpp"
#include "Shader.hpp"

#include <iostream>
#include <stdexcept>
#include <cmath>

void LearnGLApp::setup() {
    // Create the shader program
    shaderProgram = std::make_shared<Shader>("resources/shaders/shader.vert", "resources/shaders/shader.frag");

    // Let's make a square
    vertices = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,   // top left
    };
    indices = {
        0, 1, 2,
        2, 3, 0
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    // Bind the element buffer object
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // Set the data for the element buffer object
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

    // Describe the attribute layout of the data
    // location, size (items), type, normalize, stride (bytes), offset (bytes)
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coordinates (uv)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Load the image data for the texture
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);

    unsigned char *uvgridTexData = stbi_load("resources/textures/uvgrid.png", &width, &height, &channels, 0);
    if (uvgridTexData == nullptr) {
        throw std::runtime_error("Failed to load image");
    }
    // Create the texture object
    glGenTextures(1, &uvgrid);
    // Bind the texture object
    glBindTexture(GL_TEXTURE_2D, uvgrid);
    // Set a bunch of texture parameters (wrapping and filtering)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Generate the textures (type, mipmap level, texture format, width, height, 0 (border), image format, data type, data)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, uvgridTexData);
    glGenerateMipmap(GL_TEXTURE_2D);
    // Unload the image, we already stored it as a texture
    stbi_image_free(uvgridTexData);

    unsigned char *white_bearTexData = stbi_load("resources/textures/white_bear.png", &width, &height, &channels, 0);
    if (white_bearTexData == nullptr) {
        throw std::runtime_error("Failed to load image");
    }
    // Create the texture object
    glGenTextures(1, &white_bear);
    // Bind the texture object
    glBindTexture(GL_TEXTURE_2D, white_bear);
    // Set a bunch of texture parameters (wrapping and filtering)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Generate the textures (type, mipmap level, texture format, width, height, 0 (border), image format, data type, data)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, white_bearTexData);
    glGenerateMipmap(GL_TEXTURE_2D);
    // Unload the image, we already stored it as a texture
    stbi_image_free(white_bearTexData);

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

        // Activate the shader
        shaderProgram->use();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, uvgrid);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, white_bear);

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
