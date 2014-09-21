#include <iostream>
#include "Window.h"
#include "screen_manager.h"

namespace dyb
{
    using std::cout;
    using std::endl;
    using std::cerr;

    void error_callback(int error, const char* description)
    {
        std::cerr << description << endl;
    }
    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);
    }

    Window::Window(int width, int height)
        : _width(width), _height(height), screenManager(width, height)
    {
        glfwSetErrorCallback(error_callback);
        if (!glfwInit())
            exit(EXIT_FAILURE);
        window = glfwCreateWindow(_width, _height, "window", NULL, NULL);
        if (!window)
        {
            glfwTerminate();
            exit(EXIT_FAILURE);
        }
        glfwMakeContextCurrent(window);
        glfwSetKeyCallback(window, key_callback);
    }

    void Window::runLoop(LoopFunc loopFunc)
    {
        while (!glfwWindowShouldClose(window))
        {
            loopFunc();
            screenManager.drawToGL();
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
    
    Window::~Window()
    {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

}
