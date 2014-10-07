#ifndef DYB_TO_XML
#define DYB_TO_XML

#include <vector>
#include <string>
#include "glm/vec2.hpp"

#include "tmxWall.h"
#include "DijkstraAlgorithm.h"
#include "VoronoiDiagram.h"
#include "PointLocation.h"

namespace dyb
{
    using glm::ivec2;
    using std::string;
    using std::vector;

    void writeNodePosiXML(const vector<ivec2> & nodes, const string & outputXMLName, const string & mapName);
    void writeEdgeXML(const Graph & graph, const string & outputXMLName, const string & mapName);
    void writePathXML(dyb::DijkstraAlgorithm & dij, const string & outputXMLName, const string & mapName);
    void writeVoronoiXML(const dyb::VoronoiDiagram & vd, const ivec2 & mapSize,
        const string & outputXMLName, const string & mapName);
    void writeCellColumns(const dyb::PointLocation & pl,
        const string & outputXMLName, const string & mapName);
    void writePathInfoXML(const string & mapName, const ivec2 & mapSize, const string & outputXML,
        const string & tmxFile,
        const string & objectGroupName,
        const string & nodePostionXML,
        const string & edgeXML,
        const string & pathXML,
        const string & voronoiXML,
        const string & cellColumnXML
        );

}

#endif
