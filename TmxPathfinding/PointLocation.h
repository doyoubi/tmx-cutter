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

    class PointLocation
    {
    public:
        PointLocation(const VoronoiDiagram & voronoiDiagram);
        // return index of corresponding convex, which is also the index of corresponding site
        int locatePoint(glm::ivec2) const; 
        const vector<CellColumn> & getCellColumns() const { return cellColumns; }
        const vector<LineSegment> & getLineSegments() const { return lineSegments; }
        const vector<Line> & getLines() const { return lines; }

    private:
        vector<Line> lines;
        vector<LineSegment> lineSegments;
        vector<CellColumn> cellColumns;

        const VoronoiDiagram & vd;
    };
}

#endif
