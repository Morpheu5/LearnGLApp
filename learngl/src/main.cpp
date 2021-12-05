#include <iostream>
#include <stdexcept>
#include <filesystem>

#include "glad/gl.h"
#include <GLFW/glfw3.h>

#include "LearnGLApp.hpp"

int main(int argc, const char * argv[]) {
    /* Initialize the library */
    if (glfwInit() != GLFW_TRUE) {
        std::cerr << "Failed to initialize GLFW." << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    LearnGLApp app;
    try {
        makeWindow(&app, 800, 600, "LearnGL");
    } catch(std::runtime_error e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
    app.setup();
    app.run();

    return 0;
}
