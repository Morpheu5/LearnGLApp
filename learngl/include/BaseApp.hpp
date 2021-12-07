#ifndef BaseClass_h
#define BaseClass_h

#include "glad/gl.h"
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <string>
#include <fstream>
#include <iostream>

class BaseApp {
protected:
    GLFWwindow *_window = nullptr;
public:
    void window(GLFWwindow *window) { _window = window; }
    GLFWwindow *window() { return _window; }

    auto readFile(const std::string &filename) {
        std::ifstream file(filename, std::ios::binary | std::ios::ate);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to read file (" + filename + ").");
        }

        unsigned long long fileSize = (unsigned long long) file.tellg();
        char *memblock = new char[fileSize+1];
        file.seekg(0, std::ios::beg);
        file.read(memblock, fileSize);
        file.close();
        memblock[fileSize] = 0;

        return memblock;
    }
};

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

#endif
