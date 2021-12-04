#include "LearnGLApp.hpp"
#include <iostream>
#include <stdexcept>

void LearnGLApp::setup() {

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
