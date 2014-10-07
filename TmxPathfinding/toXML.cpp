#include <numeric>
#include <initializer_list>

#include "glm/glm.hpp"
#include "tinyxml2.h"

#include "toXML.h"
#include "debug.h"
#include "VoronoiDiagram.h"

namespace dyb
{
    using namespace tinyxml2;

    void writeNodePosiXML(const vector<ivec2> & nodes, const string & outputXMLName, const string & mapName)
    {
        XMLDocument doc;
        XMLDeclaration * declaration = doc.NewDeclaration();
        doc.LinkEndChild(declaration);
        XMLElement * root = doc.NewElement("NodePosition");
        doc.LinkEndChild(root);
        root->SetAttribute("mapName", mapName.c_str());
        for (size_t i = 0; i < nodes.size(); i++)
        {
            XMLElement * node = doc.NewElement("Node");
            root->LinkEndChild(node);
            node->SetAttribute("index", i);
            node->SetAttribute("x", nodes[i].x);
            node->SetAttribute("y", nodes[i].y);
        }
        doc.SaveFile(outputXMLName.c_str());
    }

    void writeEdgeXML(const Graph & graph, const string & outputXMLName, const string & mapName)
    {
        XMLDocument doc;
        XMLDeclaration * declaration = doc.NewDeclaration();
        doc.LinkEndChild(declaration);
        XMLElement * root = doc.NewElement("GraphEdge");
        doc.LinkEndChild(root);
        root->SetAttribute("mapName", mapName.c_str());
        for (int i = 0; i < graph.get_width(); i++)
        for (int j = 0; j < graph.get_width(); j++)
        {
            XMLElement * node = doc.NewElement("Edge");
            root->LinkEndChild(node);
            node->SetAttribute("start", j);
            node->SetAttribute("terminal", i);
            node->SetAttribute("distance", graph[j][i]);
        }
        doc.SaveFile(outputXMLName.c_str());
    }

    void writePathXML(dyb::DijkstraAlgorithm & dij, const string & outputXMLName, const string & mapName)
    {
        XMLDocument doc;
        XMLDeclaration * declaration = doc.NewDeclaration();
        doc.LinkEndChild(declaration);
        XMLElement * root = doc.NewElement("Path");
        doc.LinkEndChild(root);
        root->SetAttribute("mapName", mapName.c_str());
        for (int i = 0; i < dij.getNodeNumber(); i++)
        {
            XMLElement * start = doc.NewElement("StartingNode");
            root->LinkEndChild(start);
            start->SetAttribute("index", i);
            dij.run(i);
            for (int j = 0; j < dij.getNodeNumber(); j++)
            {
                if (i == j) continue;
                XMLElement * terminal = doc.NewElement("TerminalNode");
                start->LinkEndChild(terminal);
                terminal->SetAttribute("index", j);
                terminal->SetAttribute("distanceSum", dij.getPathLengthResult(j));
                for (int pathNodeIndex : dij.getPathResult(j))
                {
                    XMLElement * pathNode = doc.NewElement("PathNode");
                    terminal->LinkEndChild(pathNode);
                    pathNode->SetAttribute("index", pathNodeIndex);
                }
            }
        }
        doc.SaveFile(outputXMLName.c_str());
    }

    void writeVoronoiXML(const dyb::VoronoiDiagram & vd, const ivec2 & mapSize,
        const string & outputXMLName, const string & mapName)
    {
        using namespace glm;
        XMLDocument doc;
        XMLDeclaration * declaration = doc.NewDeclaration();
        doc.LinkEndChild(declaration);
        XMLElement * root = doc.NewElement("VoronoiDiagram");
        doc.LinkEndChild(root);
        root->SetAttribute("mapName", mapName.c_str());
        
        // write convex points
        const auto & points = vd.getConvexPoints();
        XMLElement * convexPoints = doc.NewElement("ConvexPoints");
        root->LinkEndChild(convexPoints);
        convexPoints->SetAttribute("pointNumber", points.size());
        for (int i = 0; i < points.size(); i++)
        {
            const vec2 & p = points[i];
            XMLElement * point = doc.NewElement("Point");
            convexPoints->LinkEndChild(point);
            point->SetAttribute("index", i);
            point->SetAttribute("x", p.x);
            point->SetAttribute("y", p.y);
        }
        // write point index
        XMLElement * pointIndex = doc.NewElement("PointIndex");
        root->LinkEndChild(pointIndex);
        int indexNumber = 0;
        for (int i = 0; i < vd.getConvexNum(); i++)
        {
            auto & convex = vd.getConvex(i);
            indexNumber += convex.size();
            for (int i : convex)
            {
                XMLElement * index = doc.NewElement("Index");
                pointIndex->LinkEndChild(index);
                index->SetAttribute("pointIndex", i);
            }
        }
        pointIndex->SetAttribute("indexNumber", indexNumber);
        doc.SaveFile(outputXMLName.c_str());
    }

    void writeCellColumns(const dyb::PointLocation & pl,
        const string & outputXMLName, const string & mapName)
    {
        const vector<CellColumn> & cls = pl.getCellColumns();
        const vector<LineSegment> & lines = pl.getLineSegments();

        XMLDocument doc;
        XMLDeclaration * declaration = doc.NewDeclaration();
        doc.LinkEndChild(declaration);
        XMLElement * root = doc.NewElement("PointLocation");
        doc.LinkEndChild(root);
        root->SetAttribute("mapName", mapName.c_str());

        XMLElement * cellColumnBuf = doc.NewElement("CellColumnBuf");
        root->LinkEndChild(cellColumnBuf);
        cellColumnBuf->SetAttribute("CellColumnNumber", cls.size());
        for (auto & cl : cls)
        {
            XMLElement * cellColumn = doc.NewElement("CellColumn");
            cellColumnBuf->LinkEndChild(cellColumn);
            cellColumn->SetAttribute("leftBoundX", cl.leftBound);
            cellColumn->SetAttribute("CellNumber", cl.cells.size());
            for (auto & c : cl.cells)
            {
                XMLElement * cell = doc.NewElement("Cell");
                cellColumn->LinkEndChild(cell);
                cell->SetAttribute("siteIndex", c.convexIndex);
                cell->SetAttribute("LineIndex", c.lineIndex);
            }
        }
        XMLElement * lineBuf = doc.NewElement("LineBuf");
        root->LinkEndChild(lineBuf);
        lineBuf->SetAttribute("LineNumber", lines.size());
        for (auto & l : lines)
        {
            XMLElement * line = doc.NewElement("Line");
            lineBuf->LinkEndChild(line);
            line->SetAttribute("x1", l.p1.x);
            line->SetAttribute("y1", l.p1.y);
            line->SetAttribute("x2", l.p2.x);
            line->SetAttribute("y2", l.p2.y);
        }
        doc.SaveFile(outputXMLName.c_str());
    }

    void writePathInfoXML(const string & mapName, const ivec2 & mapSize, const string & outputXML,
        const string & tmxFile,
        const string & objectGroupName,
        const string & nodePostionXML,
        const string & edgeXML,
        const string & pathXML,
        const string & voronoiXML,
        const string & cellColumnXML
        )
    {
        XMLDocument doc;
        XMLDeclaration * declaration = doc.NewDeclaration();
        doc.LinkEndChild(declaration);
        XMLElement * root = doc.NewElement("MapInfomation");
        doc.LinkEndChild(root);
        root->SetAttribute("mapName", mapName.c_str());
        root->SetAttribute("mapSizeX", mapSize.x);
        root->SetAttribute("mapSizeY", mapSize.y);

        auto addChild = [&](const string & nodeName, const string & value){
            XMLElement * node = doc.NewElement(nodeName.c_str());
            root->LinkEndChild(node);
            node->SetAttribute("value", value.c_str());
        };
        addChild("tmxFile", tmxFile);
        addChild("objectGroupName", objectGroupName);
        addChild("nodePostionXML", nodePostionXML);
        addChild("edgeXML", edgeXML);
        addChild("pathXML", pathXML);
        addChild("voronoiXML", voronoiXML);
        addChild("cellColumnXML", cellColumnXML);

        doc.SaveFile(outputXML.c_str());
    }

}