#ifndef DYB_TO_XML
#define DYB_TO_XML

#include <vector>
#include <string>
#include "glm/vec2.hpp"

#include "tmxWall.h"
#include "DijkstraAlgorithm.h"

namespace dyb
{
    using glm::ivec2;
    using std::string;
    using std::vector;

    void writeNodePosiXML(const vector<ivec2> & nodes, const string & outputXMLName, const string & mapName);
    void writeEdgeXML(const Graph & graph, const string & outputXMLName, const string & mapName);
    void writePathXML(Graph g, const string & outputXMLName, const string & mapName);
    void writeVoronoiXML(const vector<WallRect> & walls, const vector<ivec2> & nodes, const ivec2 & mapSize,
        const string & outputXMLName, const string & mapName);

}

#endif
