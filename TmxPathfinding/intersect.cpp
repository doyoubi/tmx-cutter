#include <algorithm>
#include "glm/glm.hpp"

#include "intersect.h"
#include "debug.h"

namespace dyb
{

    Graph findEdge(const vector<ivec2> & nodes, const vector<WallRect> & rects)
    {
        Graph g(nodes.size(), nodes.size());
        std::fill(std::begin(g), std::end(g), DijkstraAlgorithm::inf);
        for (int i = 0; i < int(nodes.size()); i++)
        {
            for (int j = i+1; j < int(nodes.size()); j++)
            {
                if (std::any_of(begin(rects), end(rects), [&nodes, i, j](const WallRect & r){
                    return intersect( r, Line{nodes[i], nodes[j]} );
                }))
                    continue;
                glm::vec2 delta = nodes[i] - nodes[j];
                float len = glm::length(delta);
                g[i][j] = len;
                g[j][i] = len;
            }
        }
        return g;
    }

    bool intersect(const WallRect & rect, const Line & line)
    {
        if (insideRect(rect, line.p1) || insideRect(rect, line.p2))
            return true;
        const ivec2 leftBottom = rect.leftBottom();
        const ivec2 rightTop = rect.rightTop();
        Line rectEdges[4] = {
            Line{ rect.leftTop, leftBottom },
            Line{ rect.leftTop, rightTop },
            Line{ rect.rightBottom, leftBottom },
            Line{ rect.rightBottom, rightTop },
        };
        return std::any_of(std::begin(rectEdges), std::end(rectEdges), [&line](const Line & edge){
            return intersect(edge, line);
        });
    }

    bool intersect(const Line & aaLine, const Line & line)
    {
        ivec2 a = line.p1, b = line.p2;
        int m, c, d;
        if (aaLine.p1.x == aaLine.p2.x)
        {
            m = aaLine.p1.x;
            c = std::min(aaLine.p1.y, aaLine.p2.y);
            d = std::max(aaLine.p1.y, aaLine.p2.y);
        }
        else if (aaLine.p1.y == aaLine.p2.y)
        {
            std::swap(a.x, a.y);
            std::swap(b.x, b.y);
            m = aaLine.p1.y;
            c = std::min(aaLine.p1.x, aaLine.p2.x);
            d = std::max(aaLine.p1.x, aaLine.p2.x);
        }
        else
        {
            ERRORMSG("intersect(const Line & aaLine, const Line & line) require aaLine to be axis aligned");
        }
        if ((a.x - m)*(b.x - m) > 0) return false;
        float t = float(m - b.x) / float(a.x - b.x);
        return between(c, d, int(t*(a.y - b.y) + b.y));
    }

    bool insideRect(const WallRect & rect, ivec2 p)
    {
        DEBUGCHECK(rect.leftTop.x < rect.rightBottom.x, "right should be larger than left");
        DEBUGCHECK(rect.leftTop.y < rect.rightBottom.y, "right should be larger than left");
        return between(rect.leftTop.x, rect.rightBottom.x, p.x)
            && between(rect.leftTop.y, rect.rightBottom.y, p.y);
    }

    bool between(int left, int right, int num)
    {
        DEBUGCHECK(left < right, "right should be larger than left");
        return left <= num && num <= right;
    }

    void checkNodeInsideWall(const vector<WallRect> & walls, const vector<ivec2> & nodes)
    {
        for (auto & wall : walls)
        {
            for (auto & n : nodes)
            {
                if (insideRect(wall, n))
                {
                    cout << "There is a point inside wall." << endl;
                    cout << "wall:" << endl
                        << "leftTop: " << wall.leftTop.x << ' ' << wall.leftTop.y << endl
                        << "rightBottom: " << wall.rightBottom.x << ' ' << wall.rightBottom.y << endl;
                    cout << "node: " << n.x << ',' << n.y << endl;
                    exit(1);
                }
            }
        }
    }
    
}
