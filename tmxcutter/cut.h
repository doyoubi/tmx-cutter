#ifndef DYB_CUT
#define DYB_CUT

#include <iostream>
#include <vector>
#include "point2d.h"
#include "array_2d.h"
#include "debug.h"

namespace dyb
{
    using std::cout;
    using std::cerr;
    using std::endl;
    typedef dyb::point2d<int> ivec2;
    typedef dyb::array2d<char> rect;

    void print(rect & input)
    {
        for (int y = 0; y < input.get_height(); y++)
        {
            for (int x = 0; x < input.get_width(); x++)
                cout << input[x][y];
            cout << endl;
        }
    }

    void fill_area(rect & r, int leftLowerX, int leftLowerY, int width, int height, char to_fill)
    {
        DEBUGCHECK(width > 0 && height > 0, "invalid width or height");
        DEBUGCHECK(r.is_valid_position(leftLowerX, leftLowerY),
            "invalid position");
        DEBUGCHECK(r.is_valid_position(leftLowerX + width - 1,
            leftLowerY + height - 1),
            "invalid position");
        for (int i = leftLowerY; i < leftLowerY + height; i++)
        for (int j = leftLowerX; j < leftLowerX + width; j++)
            r[j][i] = to_fill;
    }

    struct TileRect
    {
        // point (0,0) is in the left top of screen, just like the coordinate in tile map
        // 1 unit means one tile
        ivec2 leftTop, rightBottom;
    };

    struct PixelRect
    {
        // 1 unit means one pixel
        ivec2 leftTop, rightBottom;
    };

    using std::vector;
    vector<TileRect> cut(rect & input)
    {
        const int w = input.get_width(), h = input.get_height();
        rect r(input); // result

        vector<TileRect> tileRects;
        char f = 'A';
        while (1)
        {
            auto it = find(input.begin(), input.end(), '0');
            if (it == input.end()) break; // empty
            const ivec2 p = input.iterToPoint(it);

            int i = p.x, j = p.y;
            while (j + 1 < h && input[i][j + 1] != '.')
                j++;
            auto all_not_ampty = [&input](const int py, const int i, const int j){
                for (int k = py; k <= j; k++)
                if (input[i][k] == '.') return false;
                return true;
            };
            while (i + 1 < w && all_not_ampty(p.y, i + 1, j))
                i++;

            fill_area(r, p.x, p.y, i - p.x + 1, j - p.y + 1, f);
            f++;
            fill_area(input, p.x, p.y, i - p.x + 1, j - p.y + 1, '.');

            tileRects.push_back({ p, ivec2(i, j) });
            // print rectangle for debugging
            /*echoivec2(p);
            cout << i << ' ' << j << endl;
            cout << "-----------------------" << endl;*/
        }
        print(r);  // print map with tiles grouped
        return tileRects;
    }

}

#endif