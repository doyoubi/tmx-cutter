#ifndef DYB_TMX_WALL
#define DYB_TMX_WALL

#include <string>
#include <vector>
#include "glm/vec2.hpp"

namespace dyb
{
    using std::string;
    using std::vector;
    using glm::ivec2;

    // in pixel
    struct WallRect
    {
        ivec2 leftTop, rightBottom;
        ivec2 leftBottom() const { return ivec2(leftTop.x, rightBottom.y); }
        ivec2 rightTop() const { return ivec2(rightBottom.x, leftTop.y); }
    };

    vector<WallRect> parseWallXml(const string & wallXmlFile);

}

#endif
