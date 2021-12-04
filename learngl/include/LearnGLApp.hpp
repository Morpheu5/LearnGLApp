#ifndef LearnGLApp_hpp
#define LearnGLApp_hpp

#include <vector>

#include "glad/gl.h"
#include <GLFW/glfw3.h>

class LearnGLApp {
    GLFWwindow *_window;

    std::vector<float> vertices;

public:
    virtual void setup();
    virtual void run();

    void setWindow(GLFWwindow *window) { _window = window; }
    GLFWwindow *window() { return _window; }

    virtual void resize(int width, int height) {
        glViewport(0, 0, width, height);
    }

    virtual void processInput();

    virtual ~LearnGLApp();
};

#endif /* LearnGLApp_hpp */
