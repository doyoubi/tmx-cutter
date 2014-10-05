#ifndef DYB_WINDOW
#define DYB_WINDOW

#include <functional>
#include "GLFW/glfw3.h"

#include "screen_manager.h"

namespace dyb
{
    class Window
    {
    public:
        typedef std::function<void(void)> LoopFunc;
        typedef std::function<void(int, int)> MouseFunc;
        Window(int width, int height);
        ~Window();
        void setLoopFunc(LoopFunc);
        void setMouseButtonFunc(MouseFunc);
        void runLoop();
        ScreenManager * getScreenManager() { return &screenManager; }
    private:
        ScreenManager screenManager;
        int _width, _height;
        GLFWwindow* window;
        LoopFunc loopFunc = nullptr;
        MouseFunc mouseFunc = nullptr; // work with global object MouseManager
    };
}

#endif
