#ifndef LearnGLApp_hpp
#define LearnGLApp_hpp

#include <vector>

#include "BaseApp.hpp"

class LearnGLApp : public BaseApp {
    std::vector<float> vertices;

public:
    virtual void setup();
    virtual void run();

    virtual void resize(int width, int height) {
        glViewport(0, 0, width, height);
    }

    virtual void processInput();

    virtual ~LearnGLApp();
};

#endif
