#include <iostream>
#include "screen_manager.h"

namespace dyb
{
    using glm::vec3;
    using glm::ivec2;

    void ScreenManager::drawToGL()
    {
        glDrawPixels(getWidth(), getHeight(), GL_RGB, GL_FLOAT, generate_screen_image());
    }

    ScreenManager::ScreenManager(int width, int height)
        : _image(height, width)
    {
        const vec3 zero(0);
        for(int x = 0; x < _image.get_width(); x++)
        for (int y = 0; y < _image.get_height(); y++)
            _image[x][y] = zero;
        set_draw_square(0, 0, width, height);
    }

    const vec3 * ScreenManager::generate_screen_image()const
    { 
        return _image.getRawData();
    }

    void ScreenManager::set_draw_square(int left_bottom_x, int left_bottom_y, int width, int height)
    {
        if(left_bottom_x < 0 || left_bottom_y < 0 || width < 0 || height < 0
        || left_bottom_x+width > getWidth() || left_bottom_y+height > getHeight())
        {
            std::cerr<< "invalid square start point or size" <<std::endl;
            exit(1);
        }
        left_bottom_x_of_draw_square = left_bottom_x;
        left_bottom_y_of_draw_square = left_bottom_y;
        width_of_draw_square = width;
        height_of_draw_square = height;
    }

    void ScreenManager::drawPoint(const ivec2 & p, const vec3 & rgb)
    {
        if(p.x < 0 || p.x >= width_of_draw_square 
        || p.y < 0 || p.y >= height_of_draw_square)
        {
            std::cerr<< "draw out of bound!" <<std::endl;
            return;
        }
        _image[left_bottom_y_of_draw_square + p.y][left_bottom_x_of_draw_square + p.x] = rgb;
    }

    void recursiveDraw(ScreenManager * sm, const ivec2 & start, const ivec2 & end, const vec3 & rgb)
    {
        ivec2 delta = glm::abs(start - end);
        if (delta.x <= 1 && delta.y <= 1) return;
        ivec2 mid = (start + end) / 2;
        sm->drawPoint(mid, rgb);
        recursiveDraw(sm, start, mid, rgb);
        recursiveDraw(sm, mid, end, rgb);
    }

    void ScreenManager::drawLine(const ivec2 & start, const ivec2 & end, const vec3 & rgb)
    {
        drawPoint(start, rgb);
        drawPoint(end, rgb);
        recursiveDraw(this, start, end, rgb);
    }

}
