#ifndef DYB_TO_XML
#define DYB_TO_XML

#include <vector>
#include "glm/vec2.hpp"

#include "DijkstraAlgorithm.h"

namespace dyb
{
    using glm::ivec2;
    using std::vector;

    void writeNodePosiXML(const vector<ivec2> & nodes, const char * outputXMLName, const char * mapName);

    void writePathXML(Graph g, const char * outputXMLName, const char * mapName);

}

#endif
