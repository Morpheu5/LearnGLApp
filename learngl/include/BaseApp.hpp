#ifndef BaseClass_h
#define BaseClass_h

#include "glad/gl.h"
#include <GLFW/glfw3.h>
#include "stb_image.h"

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

    GLuint loadTexture(std::string filename) {
        int width, height, channels;
        GLuint textureID;
        stbi_set_flip_vertically_on_load(true);

        unsigned char *uvgridTexData = stbi_load(filename.c_str(), &width, &height, &channels, 0);
        if (uvgridTexData == nullptr) {
           throw std::runtime_error("Failed to load image");
        }
        // Create the texture object
        glGenTextures(1, &textureID);
        // Bind the texture object
        glBindTexture(GL_TEXTURE_2D, textureID);
        // Set a bunch of texture parameters (wrapping and filtering)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // Generate the textures (type, mipmap level, texture format, width, height, 0 (border), image format, data type, data)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, uvgridTexData);
        glGenerateMipmap(GL_TEXTURE_2D);
        // Unload the image, we already stored it as a texture
        stbi_image_free(uvgridTexData);

        return textureID;
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
