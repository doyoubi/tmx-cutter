#ifndef DYB_VORONOI
#define DYB_VORONOI

/*
Using a simple method to construct voronoi diagram.
There is a much faster algorithm called fortunes algorithm.
But this algorithm is quite complicated and will cause precision problem
when calculating intersection of two parabola.
Read the following article for deep understanding.
http://www.ams.org/samplings/feature-column/fcarc-voronoi
http://blog.ivank.net/fortunes-algorithm-and-implementation.html
*/

#include <vector>
#include "circular_list/circle_list.h"
#include "glm/glm.hpp"

namespace dyb
{

    class VoronoiDiagram
    {
    public:
        VoronoiDiagram(const std::vector<glm::ivec2> & _nodes, const glm::ivec2 & _mapSize);
        const circular_list<glm::vec2> & getConvex(int node) const;
        int getConvexNum() const { return convexArray.size(); }

        std::vector<circular_list<int>> convexPointIndexArray;
        std::vector<glm::ivec2> convexPoints;

    private:
        void construct();
        void constructConvexForSingleNode(int node);
        void combineConvexPoint();

        std::vector<circular_list<glm::vec2>> convexArray;
        std::vector<glm::ivec2> nodes;
        glm::ivec2 mapSize;
    };

}

#endif
