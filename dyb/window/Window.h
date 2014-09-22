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
        Window(int width, int height);
        ~Window();
        void setLoopFunc(LoopFunc);
        void runLoop();
        ScreenManager * getScreenManager() { return &screenManager; }
    private:
        ScreenManager screenManager;
        int _width, _height;
        GLFWwindow* window;
        LoopFunc loopFunc = nullptr;
    };
}

#endif
