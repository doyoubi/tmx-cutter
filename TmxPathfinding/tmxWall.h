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
    };

    vector<WallRect> parseWallXml(const string & wallXmlFile);

}

#endif
