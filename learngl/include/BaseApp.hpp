#ifndef BaseClass_h
#define BaseClass_h

#include "glad/gl.h"
#include <GLFW/glfw3.h>

class BaseApp {
protected:
    GLFWwindow *_window = nullptr;
public:
    void window(GLFWwindow *window) { _window = window; }
    GLFWwindow *window() { return _window; }
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
