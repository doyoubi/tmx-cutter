#ifndef DYB_TMX_WALL
#define DYB_TMX_WALL

#include <string>
#include <vector>
#include "../dyb/2d/array2d.h"

namespace dyb
{
    using std::string;
    using std::vector;

    typedef dyb::point2d<int> ivec2;
    typedef dyb::array2d<char> rect;

    // in pixel
    struct WallRect
    {
        ivec2 leftTop, rightBottom;
    };

    vector<WallRect> parseWallXml(const string & wallXmlFile);

}

#endif
