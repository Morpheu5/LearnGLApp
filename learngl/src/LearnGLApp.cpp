#include "LearnGLApp.hpp"
#include "Shader.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <stdexcept>
#include <cmath>

void LearnGLApp::setup() {
    int w, h;
    glfwGetWindowSize(_window, &w, &h);
    viewportSize = glm::vec2(static_cast<float>(w), static_cast<float>(h));
    lastMousePos = viewportSize / 2.0f;

    // Load the image data for the texture
    // uvgrid = loadTexture("resources/textures/uvgrid.png");
    // white_bear = loadTexture("resources/textures/white_bear.png");

    // Create the shader program
    lightingShader = std::make_shared<Shader>("resources/shaders/shader.vert", "resources/shaders/shader.frag");
    lampShader = std::make_shared<Shader>("resources/shaders/shader.vert", "resources/shaders/lampShader.frag");

    // Let's make a cube
    vertices = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    // Create vertex buffer object
    glGenBuffers(1, &VBO);

    // Create vertex array object
    glGenVertexArrays(1, &cubeVAO);
    // Bind vertex buffer object
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Set data for the VBO
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(float) * vertices.size()), vertices.data(), GL_STATIC_DRAW);

    // Bind vertex array object
    glBindVertexArray(cubeVAO);
    // Describe the attribute layout of the data
    // location, size (items), type, normalize, stride (bytes), offset (bytes)
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), static_cast<const void *>(0));
    glEnableVertexAttribArray(0);
    // texture coordinates (uv)
    // glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<const void *>(3 * sizeof(float)));
    // glEnableVertexAttribArray(1);

    glGenVertexArrays(1, &lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindVertexArray(lightVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), static_cast<const void *>(0));
    glEnableVertexAttribArray(0);

    // lightingShader->use();
    // Use the locations, not the texture IDs.
    // lightingShader->setInt("uvgrid", 0);
    // lightingShader->setInt("white_bear", 1);

    // The VBO is already registered so we can safely unbind here
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Unbind the VAO so it doesn't get accidentally modified
    glBindVertexArray(0);

    // This one renders in wireframe
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glEnable(GL_DEPTH_TEST);

    glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void LearnGLApp::run() {
    while(!glfwWindowShouldClose(_window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput();
        
        // Render stuff
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // float t = glfwGetTime();

        // View (camera), move it backwards a little
        glm::mat4 view = camera.GetViewMatrix();

        // Projection (perspective)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), viewportSize.x/viewportSize.y, 0.1f, 100.0f);

        /*
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, uvgrid);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, white_bear);
         */

        {
            glBindVertexArray(cubeVAO);
            glm::mat4 model(1.0f);

            lightingShader->use();
            lightingShader->setVec3f("objectColor", { 0.0f, 0.667f, 1.0f });
            lightingShader->setVec3f("lightColor", { 1.0f, 1.0f, 1.0f });
            lightingShader->setMat4f("view", view);
            lightingShader->setMat4f("projection", projection);
            lightingShader->setMat4f("model", model);

            glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices.size()));
        }

        {
            glBindVertexArray(lightVAO);
            glm::mat4 model(1.0f);
            model = glm::translate(model, lightPosition);
            model = glm::scale(model, glm::vec3(0.2f));

            lampShader->use();
            lampShader->setMat4f(("view"), view);
            lampShader->setMat4f("projection", projection);
            lightingShader->setMat4f("model", model);

            glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices.size()));
        }

        // Do the framebuffer swappy thing
        glfwSwapBuffers(_window);

        // Handle events
        glfwPollEvents();
    }
}

void LearnGLApp::mouseMoved(float xPos, float yPos) {
    if (firstMouseEvent) {
        lastMousePos = glm::vec2(xPos, yPos);
        firstMouseEvent = false;
    }

    const float xOff = xPos - lastMousePos.x;
    const float yOff = lastMousePos.y - yPos; // upside down
    lastMousePos = glm::vec2(xPos, yPos);

    camera.ProcessMouseMovement(xOff, yOff);
}

void LearnGLApp::mouseScrolled(float xOffset, float yOffset) {
    camera.ProcessMouseScroll(yOffset);
}

void LearnGLApp::processInput() {
    if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(_window, GLFW_TRUE);
    }

    if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}


LearnGLApp::~LearnGLApp() {
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightVAO);
    glfwTerminate();
}
