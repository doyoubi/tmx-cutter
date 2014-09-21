#ifndef DYB_WINDOW
#define DYB_WINDOW

#include "GLFW/glfw3.h"

#include "screen_manager.h"

namespace dyb
{
    class Window
    {
    public:
        Window(int width, int height);
        ~Window();
        typedef void (*LoopFunc)();
        void runLoop(LoopFunc);
        ScreenManager * getScreenManager() { return &screenManager; }
    private:
        ScreenManager screenManager;
        int _width, _height;
        GLFWwindow* window;
    };
}

#endif
