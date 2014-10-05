#include <algorithm>
#include <map>
#include <set>
#include <list>

#include "PointLocation.h"
#include "circular_list/circle_list.h"
#include "2d\array2d.h"

namespace dyb
{
    using glm::ivec2;

    PointLocation::PointLocation(const VoronoiDiagram & voronoiDiagram)
        : vd(voronoiDiagram)
    {
        const vector<ivec2> nodes = vd.getNodes();
        const vector<ivec2> convexPoints = vd.getConvexPoints();
        // get lines and lineSegments
        array2d<int> tag(convexPoints.size(), convexPoints.size());
        std::fill(std::begin(tag), std::end(tag), 0);
        for (int i = 0; i < vd.getConvexNum(); i++)
        {
            auto convex = vd.getConvex(i);
            dyb::for_adjacent(convex.loop_begin(), convex.loop_end(),
                [&](int curr, int next){
                if (tag[curr][next] || tag[next][curr]) return;
                tag[curr][next] = tag[next][curr] = 1;
                ivec2 left = convexPoints[curr], right = convexPoints[next];
                if (left.x > right.x) std::swap(left, right);
                lineSegments.push_back(LineSegment{left, right});
                lines.push_back(point2Line(left, right));
            });
        }
        // contruct two list. one for add line segment, one for delete
        using std::map;
        using std::set;
        map<int, vector<int>> addList;
        map<int, vector<int>> deleleList;
        set<int> eventX;
        for (size_t i = 0; i < lineSegments.size(); i++)
        {
            LineSegment & l = lineSegments[i];
            ivec2 left = l.p1, right = l.p2;
            eventX.insert(left.x);
            eventX.insert(right.x);
            if (left.x == right.x) continue; // do not add vertical line
            addList[left.x].push_back(i);
            deleleList[right.x].push_back(i);
        }
        DEBUGCHECK(eventX.find(0) != end(eventX), "0 should exist in eventX");

        // scan from left to right
        using std::list;
        list<int> columnLine;
        vector<int> columnLeftX(begin(eventX), end(eventX));
        for (size_t i = 0; i < columnLeftX.size(); i++)
        {
            if (i == columnLeftX.size() - 1) break; // exclude last

            int x = columnLeftX[i];
            cellColumns.push_back(CellColumn{ vector<Cell>(), x });
            for (int l : deleleList[x])
            {
                columnLine.remove(l);
            }
            auto y = [this](float x, int line) -> float {
                auto & l = lineSegments[line];
                return float(l.p2.y - l.p1.y) / float(l.p2.x - l.p1.x) * (x - l.p1.x) + l.p1.y;
            }; // find y for a given line segment and x
            for (int l : addList[x])
            {
                auto it = std::lower_bound(begin(columnLine), end(columnLine), l,
                    [&](int l1, int l2){
                    int rx = columnLeftX[i + 1];
                    if (y(x, l1) == y(x, l2)) return y(rx, l1) < y(rx, l2);
                    return y(x, l1) < y(x, l2);
                });
                columnLine.insert(it, l);
            }
            // fill corresponding node in cell
            float mx = float(x + columnLeftX[i + 1]) / 2.0f;
            vector<int> tempColumnLine(begin(columnLine), end(columnLine));
            for (int j = 0; j < tempColumnLine.size() - 1; ++j) // exclude the last one
            {
                int line = tempColumnLine[j];
                int upperLine = tempColumnLine[j + 1];
                float my = (y(mx, line) + y(mx, upperLine)) / 2.0f;
                vec2 p(mx, my);
                auto it = std::min_element(begin(nodes), end(nodes),
                    [&](ivec2 i, ivec2 smallest){
                    return glm::length(vec2(i) - p) < glm::length(vec2(smallest) - p);
                });
                size_t n = it - begin(nodes);
                cellColumns.back().cells.push_back(Cell{ n, line });
            }
        }
    }

    int PointLocation::locatePoint(ivec2 point) const
    {
        auto y = [this](float x, int line) -> float {
            auto & l = lineSegments[line];
            return float(l.p2.y - l.p1.y) / float(l.p2.x - l.p1.x) * (x - l.p1.x) + l.p1.y;
        }; // find y for a given line segment and x

        auto i = std::upper_bound(begin(cellColumns), end(cellColumns), point.x,
            [](int x, const CellColumn & lhs){
            return x < lhs.leftBound;
        });
        // what we find is not i, but --i
        const CellColumn & cl = cellColumns[i - begin(cellColumns) - 1];
        // begin(cellColumns)->leftBound is always 0, so when using upper_bound to find i,
        // it should never be the first one

        auto j = std::upper_bound(begin(cl.cells), end(cl.cells), point.y,
            [&](int pointY, const Cell & c){
            return pointY < y(point.x, c.lineIndex);
        });
        // what we find is not j, but --j
        const Cell & c = cl.cells[j - begin(cl.cells) - 1];
        return c.convexIndex;
    }

}
