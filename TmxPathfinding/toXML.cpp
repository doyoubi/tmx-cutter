#include "tinyxml2.h"

#include "toXML.h"
#include "debug.h"


namespace dyb
{
    using namespace tinyxml2;

    void writeNodePosiXML(const vector<ivec2> & nodes, const char * outputXMLName, const char * mapName)
    {
        XMLDocument doc;
        XMLDeclaration * declaration = doc.NewDeclaration();
        doc.LinkEndChild(declaration);
        XMLElement * root = doc.NewElement("NodePosition");
        doc.LinkEndChild(root);
        root->SetAttribute("mapName", mapName);
        for (size_t i = 0; i < nodes.size(); i++)
        {
            XMLElement * node = doc.NewElement("Node");
            root->LinkEndChild(node);
            node->SetAttribute("index", i);
            node->SetAttribute("x", nodes[i].x);
            node->SetAttribute("y", nodes[i].y);
        }
        doc.SaveFile(outputXMLName);
    }

    void writePathXML(Graph g, const char * outputXMLName, const char * mapName)
    {
        DijkstraAlgorithm dij(std::move(g));

        XMLDocument doc;
        XMLDeclaration * declaration = doc.NewDeclaration();
        doc.LinkEndChild(declaration);
        XMLElement * root = doc.NewElement("Path");
        doc.LinkEndChild(root);
        root->SetAttribute("mapName", mapName);
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
        doc.SaveFile(outputXMLName);
    }

}