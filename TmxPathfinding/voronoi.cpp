#include <vector>
#include "glm/glm.hpp"
#include "debug.h"
#include "voronoi.h"

namespace dyb
{
    using std::vector;
    using glm::vec2;
    using glm::vec3;
    using glm::ivec2;
    using dyb::circular_list;

    struct Line{ float a, b, c; };

    Line point2Line(const vec2 & p1, const vec2 & p2)
    {
        vec3 a = vec3(p2 - p1, 0);
        vec3 b = vec3(p1, 1);
        vec3 n = glm::cross(a, b);
        return Line{ n.x, n.y, n.z };
    }

    Line findPerpendicularBisector(const vec2 & p1, const vec2 & p2)
    {
        float x1 = p1.x, y1 = p1.y, x2 = p2.x, y2 = p2.y;
        return Line{ 2 * (x1 - x2), 2 * (y1 - y2), x2*x2 - x1*x1 + y2*y2 - y1*y1 };
    }

    vec2 lineItersection(const Line & l1, const Line & l2)
    {
        vec3 s1(l1.a, l1.b, l1.c), s2(l2.a, l2.b, l2.c);
        vec3 s = glm::cross(s1, s2);
        DEBUGCHECK(s.z != 0.0f, "two lines are parallel");
        return vec2(s.x / s.z, s.y / s.z);
    }

    VoronoiDiagram::VoronoiDiagram(const vector<ivec2> & _nodes, const ivec2 & _mapSize)
        : nodes(_nodes), mapSize(_mapSize), convexArray(_nodes.size()),
        convexPointIndexArray(_nodes.size())
    {
        DEBUGCHECK(_mapSize.x > 0 && _mapSize.y > 0, "invalid size");
        DEBUGCHECK(_nodes.size() > 1, "invalid nodes number");
        construct();
    }

    const circular_list<int> & VoronoiDiagram::getConvex(int node) const
    {
        DEBUGCHECK(0 <= node && node < convexArray.size(), "invalid node index");
        return convexPointIndexArray[node];
    }

    void VoronoiDiagram::construct()
    {
        for (int i = 0; i < nodes.size(); i++)
        {
            constructConvexForSingleNode(i);
        }
        combineConvexPoint();
    }

    void VoronoiDiagram::constructConvexForSingleNode(int node)
    {
        DEBUGCHECK(0 <= node && node < nodes.size(), "invalid node index");
        circular_list<vec2> & convex = convexArray[node];
        vector<vec2> initNode = {
            vec2(0.0f, 0.0f), vec2(mapSize.x - 1, 0.0f), vec2(mapSize.x - 1, mapSize.y - 1), vec2(0.0f, mapSize.y - 1),
        };
        for (auto & n : initNode)
        {
            convex.insert(std::end(convex), n);
        }
        for (ivec2 & p : nodes)
        {
            if (p == nodes[node]) continue;
            //cout << p.x << ' ' << p.y << endl;

            Line l = findPerpendicularBisector(nodes[node], p);
            auto eq = [&l](const vec2 & p){ return l.a * p.x + l.b * p.y + l.c; };
            float eqStart = eq(nodes[node]);
            // find such a, a and start are on the same side while next of a not 
            auto a = dyb::adjacent_find(convex.loop_begin(), convex.loop_end(),
                [&eq, eqStart](const vec2 & first, const vec2 & next){
                return eq(first) * eqStart > 0  // same side 
                    && eq(next) * eqStart <= 0; // different side, here we should use <= rather than <
            });
            if (a == std::end(convex)) continue; // no intersection
            // find such b, next of b and start are on the same side while b not
            // if a exist, b must exist either, 
            // because Line l shouldn't overlap any border of convex, or some point are on the same positin
            // which must not happen
            auto b = dyb::adjacent_find(convex.loop_begin(), convex.loop_end(),
                [&eq, eqStart](const vec2 & first, const vec2 & next){
                return eq(first) * eqStart <= 0 // must use <= rather than <
                    && eq(next) * eqStart > 0;
            });
            DEBUGCHECK(b != std::end(convex), "b does not exist.");
            auto c = a; ++c;
            auto d = b; ++d;
            // intersetction point,
            // because a and c are on the different side, so there must be intersection point
            vec2 its1 = lineItersection(l, point2Line(*a, *c)); // construct a -> e -> c
            vec2 its2 = lineItersection(l, point2Line(*b, *d));
            auto e = convex.insert(c, its1);
            auto f = convex.insert(d, its2);
            while (1) // erase from c to b
            {
                auto next = e;
                ++next;
                if (next == f) break;
                convex.erase(next);
            }
        }
    }

    void VoronoiDiagram::combineConvexPoint()
    {
        // called after constructConvexForSingleNode() for all node
        DEBUGCHECK(convexArray.size() == convexPointIndexArray.size(), "should have the same size");
        const float threshold = 3;
        vector<vec2> temp;
        for (int i = 0; i < convexArray.size(); i++)
        {
            const circular_list<vec2> & convex = convexArray[i];
            DEBUGCHECK(convex.size() > 2, "invalid convex");
            circular_list<int> & convexPointIndex = convexPointIndexArray[i]; // empty
            for (const vec2 & p : convex)
            {
                auto it = std::find_if(begin(temp), end(temp), [&](const vec2 & point){
                    return glm::length(p - point) < threshold;
                });
                if (it != end(temp)) // already exist
                {
                    *it = (*it + p) / 2.0f;
                    convexPointIndex.insert(std::end(convexPointIndex), it - std::begin(temp));
                }
                else
                {
                    temp.push_back(p);
                    convexPointIndex.insert(std::end(convexPointIndex), temp.size() - 1);
                }
            }
            DEBUGCHECK(convex.size() == convexPointIndex.size(), "wrong result");
        }
        for (vec2 & p : temp)
            convexPoints.push_back(ivec2(p));
    }

}