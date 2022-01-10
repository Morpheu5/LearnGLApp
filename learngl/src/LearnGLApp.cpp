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

    // Create the shader program
    phongShader = std::make_shared<Shader>("resources/shaders/shader.vert", "resources/shaders/phongShader.frag");
    lampShader = std::make_shared<Shader>("resources/shaders/shader.vert", "resources/shaders/lampShader.frag");
    diffuseMap = loadTexture("resources/textures/container2.png");
    specularMap = loadTexture("resources/textures/container2_specular.png");
    
    // Let's make a cube
    vertices = {
        // positions          normals              uv
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    // Create vertex buffer object
    glGenBuffers(1, &VBO);
    // Bind vertex buffer object
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Set data for the VBO
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(float) * vertices.size()), vertices.data(), GL_STATIC_DRAW);

    // Create vertex array object
    glGenVertexArrays(1, &cubeVAO);
    // Bind vertex array object
    glBindVertexArray(cubeVAO);
    // Describe the attribute layout of the data
    // location, size (items), type, normalize, stride (bytes), offset (bytes)
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), static_cast<const void *>(0));
    glEnableVertexAttribArray(0);
    // normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<const void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coordinates (uv)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<const void *>(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glGenVertexArrays(1, &lightVAO);
    // No need to rebind the VBO
    // glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindVertexArray(lightVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), static_cast<const void *>(0));
    glEnableVertexAttribArray(0);

    // The VBO is already registered so we can safely unbind here
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Unbind the VAO so it doesn't get accidentally modified
    glBindVertexArray(0);

    // This one renders in wireframe
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

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

        float t = glfwGetTime();

        // View (camera), move it backwards a little
        glm::mat4 view = camera.GetViewMatrix();

        // Projection (perspective)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), viewportSize.x/viewportSize.y, 0.1f, 100.0f);

        glm::vec3 lightColor { 1.0f, 1.0f, 1.0f };
        glm::vec3 lightDiffuseColor = lightColor * 0.5f;
        glm::vec3 lightAmbientColor = lightColor * 0.2f;
        lightPosition.x = 0.5f * sinf(t);
        lightPosition.z = 0.5f * cosf(t);

        {
            glBindVertexArray(cubeVAO);
            glm::mat4 model(1.0f);
            // model = glm::translate(model, glm::vec3(0.5f * sinf(t), 0.0f, 0.5f * cosf(t)));

            phongShader->use();
            phongShader->setInt("material.diffuse", 0);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, diffuseMap);
            phongShader->setInt("material.specular", 1);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, specularMap);

            phongShader->setVec3f("material.specular", { 0.5f, 0.5f, 0.5f });
            phongShader->setFloat("material.shininess", 32.0f);

            phongShader->setVec3f("light.ambient", lightAmbientColor);
            phongShader->setVec3f("light.diffuse", lightDiffuseColor);
            phongShader->setVec3f("light.specular", { 1.0f, 1.0f, 1.0f });

            phongShader->setFloat("light.constant", 1.0f);
            phongShader->setFloat("light.linear", 0.09f);
            phongShader->setFloat("light.quadratic", 0.032f);

            phongShader->setVec3f("light.position", lightPosition);
            phongShader->setVec3f("cameraPosition", camera.Position);
            phongShader->setMat4f("model", model);
            phongShader->setMat4f("view", view);
            phongShader->setMat4f("projection", projection);
            phongShader->setMat4f("mvp", projection * view * model);
            phongShader->setMat3f("normalMatrix", glm::transpose(glm::inverse(model)));


            glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices.size()));
        }

        {
            glBindVertexArray(lightVAO);
            glm::mat4 model(1.0f);
            model = glm::translate(model, lightPosition);
            model = glm::scale(model, glm::vec3(0.2f));

            lampShader->use();
            lampShader->setVec3f("color", lightColor);
            lampShader->setMat4f("model", model);
            lampShader->setMat4f(("view"), view);
            lampShader->setMat4f("projection", projection);
            lampShader->setMat4f("mvp", projection * view * model);
            lampShader->setMat3f("normalMatrix", glm::transpose(glm::inverse(model)));

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
    if (glfwGetKey(_window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);
    if (glfwGetKey(_window, GLFW_KEY_E) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
}


LearnGLApp::~LearnGLApp() {
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightVAO);
    glfwTerminate();
}
