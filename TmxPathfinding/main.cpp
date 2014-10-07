#include <tuple>
#include <iostream>
#include <memory>
#include <string>
//#include "test.h"
#include "GLFW/glfw3.h"
#include "glm/vec2.hpp"
#include "../TmxParser/Tmx.h"
#include "../TmxParser/TinyXML/tinyxml.h"

#include "debug.h"
#include "window/Window.h"
#include "DijkstraAlgorithm.h"
#include "VoronoiDiagram.h"
#include "tmxWall.h"
#include "tmxObjNode.h"
#include "intersect.h"
#include "toXML.h"
#include "PointLocation.h"
using std::cout;
using std::endl;
using std::string;
using std::shared_ptr;
using glm::vec2;
using glm::ivec2;
using glm::vec3;
using dyb::debugCheck;

void findPathDebugDisplay(const std::vector<ivec2> & nodes, const std::vector<dyb::WallRect> & walls,
    const dyb::Graph & graph, dyb::Window & win);
void voronoiDebugDisplay(const dyb::VoronoiDiagram & vd, const std::vector<ivec2> & nodes, dyb::Window & win);
void pointLocationDebugDisplay(const dyb::PointLocation & pl,
    const std::vector<ivec2> & nodes,
    dyb::Window & win);

int main()
{
    const string wallXmlFile = "resources/road1_wall.xml";
    const string tmxFile = "resources/road1.tmx";
    const string objectGroupName = "path";
    const string nodePostionXML = "resources/nodePosition.xml";
    const string edgeXML = "resources/edge.xml";
    const string pathXML = "resources/path.xml";
    const string voronoiXML = "resources/voronoi.xml";
    const string cellColumnXML = "resources/cellColumn.xml";
    const string pathInfoXML = "resources/pathInfo.xml";
    const string mapName = "road1";

    Tmx::Map map;
    map.ParseFile(tmxFile);
    if (map.HasError())
    {
        cout << "error code : " << map.GetErrorCode() << endl;
        cout << "error text: " << map.GetErrorText() << endl;
    }

    auto nodes = dyb::getTmxObjNode(map, objectGroupName);
    auto t = dyb::parseWallXml(wallXmlFile);
    auto walls = std::get<0>(t);
    const ivec2 mapSize = std::get<1>(t);
    DEBUGCHECK(mapSize.x == map.GetWidth() * map.GetTileWidth()
        && mapSize.y == map.GetHeight() * map.GetTileHeight(),
        "size in wall xml file is not the same as the size in map tmx file.");
    dyb::checkNodeInsideWall(walls, nodes);
    auto graph = dyb::findEdge(nodes, walls);

    dyb::Window win(mapSize.x, mapSize.y);

    dyb::writeNodePosiXML(nodes, nodePostionXML, mapName);
    dyb::writeEdgeXML(graph, edgeXML, mapName);
    dyb::DijkstraAlgorithm dij(graph);
    dyb::writePathXML(dij, pathXML, mapName);
    dyb::VoronoiDiagram vd(nodes, mapSize);
    dyb::writeVoronoiXML(vd, mapSize, voronoiXML, mapName);
    dyb::PointLocation pl(vd);
    dyb::writeCellColumns(pl, cellColumnXML, mapName);
    dyb::writePathInfoXML(mapName, mapSize, pathInfoXML,
        tmxFile, objectGroupName, nodePostionXML, edgeXML, pathXML, voronoiXML, cellColumnXML);
    
    win.runLoop();

    return 0;
}


void drawAARect(dyb::Window & win, const ivec2 & leftTop, const ivec2 & rightBottom, const vec3 & rgb)
{
    auto sm = win.getScreenManager();
    const ivec2 rightTop(rightBottom.x, leftTop.y);
    const ivec2 leftBottom(leftTop.x, rightBottom.y);
    sm->drawLine(leftTop, rightTop, rgb);
    sm->drawLine(leftTop, leftBottom, rgb);
    sm->drawLine(rightBottom, leftBottom, rgb);
    sm->drawLine(rightBottom, rightTop, rgb);
}

void findPathDebugDisplay(const std::vector<ivec2> & nodes, const std::vector<dyb::WallRect> & walls, 
    const dyb::Graph & graph, dyb::Window & win)
{
    const vec3 red(1, 0, 0);
    for (auto & n : nodes)
    {
        win.getScreenManager()->drawPoint(n, vec3(0, 0, 1));
    }
    for (auto & w : walls)
    {
        drawAARect(win, w.leftTop, w.rightBottom, red);
    }
    for (int i = 0; i < graph.get_width(); i++)
    {
        for (int j = 0; j < graph.get_height(); j++)
        {
            if (graph[i][j] == dyb::DijkstraAlgorithm::inf)
                continue;
            win.getScreenManager()->drawLine(nodes[i], nodes[j], vec3(0, 0, 1));
        }
    }
    dyb::DijkstraAlgorithm dij(graph);
    dij.run(0);
    for (size_t pre = 0, curr = 1; curr < dij.getPathResult(nodes.size()-1).size(); ++pre, ++curr)
    {
        const ivec2 & preNode = nodes[dij.getPathResult(11)[pre]];
        const ivec2 & currNode = nodes[dij.getPathResult(11)[curr]];
        win.getScreenManager()->drawLine(preNode, currNode, vec3(0, 1, 0));
    }
}

void voronoiDebugDisplay(const dyb::VoronoiDiagram & vd, const std::vector<ivec2> & nodes, dyb::Window & win)
{
    auto & points = vd.getConvexPoints();
    for (int i = 0; i < vd.getConvexNum(); ++i)
    {
        //auto & convex = vd.getConvex(i);
        auto & convex = vd.getConvex(i);
        auto it = convex.loop_begin();
        auto next = it; ++next;
        do
        {
            win.getScreenManager()->drawLine(points[*it], points[*next], vec3(1, 1, 0));
            ++it; ++next;
        } while (it != convex.loop_end());
        win.getScreenManager()->drawPoint(nodes[i], vec3(0, 0, 1));
    }
}

void pointLocationDebugDisplay(const dyb::PointLocation & pl,
    const std::vector<ivec2> & nodes,
    dyb::Window & win)
{
    /*cout << "enter cell column index: ";
    int k; std::cin >> k;
    int j = pl.cellColumns[k].leftBound;
    for (auto & c : pl.cellColumns[k].cells)
    {
    int l = c.lineIndex;
    auto line = pl.lineSegments[l];
    win.getScreenManager()->drawLine(line.p1, line.p2, vec3(0, 1, 0));
    win.getScreenManager()->drawLine(
    ivec2(j, 0), ivec2(j, win.getScreenManager()->getHeight()-1), vec3(0, 0, 1)
    );}*/
    for (auto l : pl.getLineSegments())
    {
        win.getScreenManager()->drawLine(l.p1, l.p2, vec3(1, 0, 0));
    }
    win.setMouseButtonFunc([&](int x, int y){
        win.getScreenManager()->drawPoint(ivec2(x, y), vec3(1, 1, 0));
        int i = pl.locatePoint(ivec2(x, y));
        ivec2 n = nodes[i];
        cout << n.x << ' ' << n.y << endl;
        win.getScreenManager()->drawPoint(n, vec3(0, 1, 0));
    });
}
