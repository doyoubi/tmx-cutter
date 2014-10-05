#ifndef DYB_LINE
#define DYB_LINE    

#include "glm/glm.hpp"

namespace dyb
{
    using glm::vec2;
    using glm::ivec2;
    using glm::vec3;


    struct LineSegment
    {
        ivec2 p1, p2;
    };

    struct Line{ float a, b, c; };

    inline Line point2Line(const vec2 & p1, const vec2 & p2)
    {
        vec3 a = vec3(p2 - p1, 0);
        vec3 b = vec3(p1, 1);
        vec3 n = glm::cross(a, b);
        return Line{ n.x, n.y, n.z };
    }

    inline Line findPerpendicularBisector(const vec2 & p1, const vec2 & p2)
    {
        float x1 = p1.x, y1 = p1.y, x2 = p2.x, y2 = p2.y;
        return Line{ 2 * (x1 - x2), 2 * (y1 - y2), x2*x2 - x1*x1 + y2*y2 - y1*y1 };
    }

}

#endif
