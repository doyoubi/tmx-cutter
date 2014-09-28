#include <numeric>

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

    void writePathXML(Graph g, const string & outputXMLName, const string & mapName)
    {
        DijkstraAlgorithm dij(std::move(g));

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

}