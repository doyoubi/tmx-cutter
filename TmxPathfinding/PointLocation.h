#ifndef DYB_POINT_LOCATION
#define DYB_POINT_LOCATION

#include <vector>
#include "glm/glm.hpp"

#include "VoronoiDiagram.h"
#include "Line.h"

namespace dyb
{
    using std::vector;

    struct Cell
    {
        int convexIndex; // same as node index
        int lineIndex;
    };

    struct CellColumn
    {
        vector<Cell> cells;
        int leftBound;
    };

    struct PointLocation
    {
        PointLocation(const VoronoiDiagram & voronoiDiagram);
        int locatePoint(glm::ivec2) const;

        vector<Line> lines;
        vector<LineSegment> lineSegments;
        vector<CellColumn> cellColumns;

        const VoronoiDiagram & vd;
    };
}

#endif
