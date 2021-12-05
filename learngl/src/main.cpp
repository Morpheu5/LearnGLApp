#include <iostream>
#include <stdexcept>
#include <filesystem>

#include "glad/gl.h"
#include <GLFW/glfw3.h>

#include "LearnGLApp.hpp"

template <class T>
void makeWindow(T *userWindow, int width, int height, const char *title) {
    GLFWwindow *glfwWindow = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (glfwWindow == nullptr) {
        throw std::runtime_error("Failed to create a GLFW window");
    }
    static_cast<BaseApp *>(userWindow)->window(glfwWindow);

    glfwMakeContextCurrent(glfwWindow);
    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
        throw std::runtime_error("Failed to initialize GLAD");
    }
    glViewport(0, 0, width, height);

    glfwSetWindowUserPointer(glfwWindow, userWindow);

    auto _resize = [](GLFWwindow *w, int width, int height) { static_cast<T *>(glfwGetWindowUserPointer(w))->resize(width, height); };
    glfwSetFramebufferSizeCallback(glfwWindow, _resize);
}

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
