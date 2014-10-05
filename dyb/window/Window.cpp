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

    // global object
    struct MouseManager
    {
        bool mouseClick = false;
        int mouseClickX, mouseClickY;
    } dybMouseManager;

    void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
    {
        if (action == GLFW_PRESS)
            dybMouseManager.mouseClick = true;
        else return;
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        dybMouseManager.mouseClickX = (int)x;
        dybMouseManager.mouseClickY = (int)y;
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
        glfwSetMouseButtonCallback(window, mouse_button_callback);
    }

    void Window::setLoopFunc(LoopFunc LoopFunction)
    {
        loopFunc = LoopFunction;
    }

    void Window::setMouseButtonFunc(MouseFunc mouseFunction)
    {
        mouseFunc = mouseFunction;
    }

    void Window::runLoop()
    {
        while (!glfwWindowShouldClose(window))
        {
            if (loopFunc)
                loopFunc();
            if (mouseFunc && dybMouseManager.mouseClick)
            {
                dybMouseManager.mouseClick = false;
                mouseFunc(dybMouseManager.mouseClickX,
                    _height - dybMouseManager.mouseClickY);
            }
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
