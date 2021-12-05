#ifndef BaseClass_h
#define BaseClass_h

class BaseApp {
protected:
    GLFWwindow *_window = nullptr;
public:
    void window(GLFWwindow *window) { _window = window; }
    GLFWwindow *window() { return _window; }
};

#endif
