#include <iostream>
#include <stdexcept>
#include <filesystem>

#include "glad/gl.h"
#include <GLFW/glfw3.h>

#include "LearnGLApp.hpp"

auto main(int argc, const char * argv[]) -> int {
    /* Initialize the library */
    if (glfwInit() != GLFW_TRUE) {
        std::cerr << "Failed to initialize GLFW." << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    // Let's splurge
    glfwWindowHint(GLFW_SAMPLES, 16);
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_TRUE);

    LearnGLApp app;
    try {
        makeWindow(&app, 800, 600, "LearnGL");
        app.setup();
        app.run();
    } catch (std::runtime_error e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }

    return 0;
}
