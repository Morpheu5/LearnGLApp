#ifndef BaseClass_h
#define BaseClass_h

#include "glad/gl.h"
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <string>
#include <fstream>

class BaseApp {
protected:
    GLFWwindow *_window = nullptr;
public:
    void window(GLFWwindow *window) { _window = window; }
    GLFWwindow *window() { return _window; }

    auto readFile(const std::string &filename) {
        std::ifstream file(filename, std::ios::ate | std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to read file (" + filename + ").");
        }

        unsigned long long fileSize = (unsigned long long) file.tellg();
        std::vector<char> *buffer = new std::vector<char>(fileSize);
        file.seekg(0);
        file.read(buffer->data(), fileSize);
        file.close();

        return buffer;
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
