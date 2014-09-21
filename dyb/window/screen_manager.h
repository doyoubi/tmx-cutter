#ifndef _SCREEN_MANAGER_H
#define _SCREEN_MANAGER_H

#include <iostream>
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "../dyb/2d/array2d.h"

namespace dyb
{
    typedef dyb::array2d<glm::vec3> img;

    class ScreenManager
    {
        public:
            ScreenManager(int width, int height);
            // default is full screen
            void set_draw_square(int left_bottom_x, int left_bottom_y, int width, int height);
            void drawPoint(const glm::ivec2 & point, const glm::vec3 & rgb);
            void drawLine(const glm::ivec2 & start, const glm::ivec2 & end, const glm::vec3 & rgb);
            void drawToGL();

            int getWidth() const { return _image.get_height(); }
            int getHeight() const { return _image.get_width(); }

            const glm::vec3 * generate_screen_image()const;
        private:
            img _image;
            int left_bottom_x_of_draw_square, left_bottom_y_of_draw_square;
            int width_of_draw_square, height_of_draw_square;
    };

}

#endif
