#ifndef DYB_INTERSECT
#define DYB_INTERSECT

#include <vector>
#include "glm/vec2.hpp"

#include "tmxWall.h"
#include "2d/array2d.h"
#include "DijkstraAlgorithm.h"
#include "Line.h"

namespace dyb
{
    using glm::ivec2;
    using std::vector;


    Graph findEdge(const vector<ivec2> & nodes, const vector<WallRect> & rects);

    bool intersect(const WallRect & wallRect, const LineSegment & line);
    bool intersect(const LineSegment & aaLineSegment, const LineSegment & line);
    bool insideRect(const WallRect & wallRect, ivec2 p);
    bool between(int left, int right, int num);

    // if there is a node inside wall, it will print the location and stop the program
    void checkNodeInsideWall(const vector<WallRect> & walls, const vector<ivec2> & nodes);
}

#endif
