#include <iostream>
#include <algorithm>
#include <memory>
#include <string>
#include <sstream>
#include <fstream>
//#include "test.h"
#include "../TmxParser/Tmx.h"
#include "../TmxParser/TinyXML/tinyxml.h"
#include "../dyb/debug.h"
#include "DijkstraAlgorithm.h"
#include "tmxWall.h"
using std::cout;
using std::endl;
using std::string;
using std::shared_ptr;

typedef dyb::point2d<int> ivec2;
typedef dyb::array2d<char> rect;

int main()
{
    const string wallXmlFile = "road1_wall.xml";
    const string tmxFile = "road1.tmx";
    const string objectGroupName = "path";
    const string savedXmlFile = "output.xml";

    shared_ptr<Tmx::Map> map(new Tmx::Map());
    map->ParseFile(tmxFile);
    if (map->HasError())
    {
        cout << "error code : " << map->GetErrorCode() << endl;
        cout << "error text: " << map->GetErrorText() << endl;
        return map->GetErrorCode();
    }

    Tmx::ObjectGroup * pathObjGroup = *std::find_if(
        begin(map->GetObjectGroups()), end(map->GetObjectGroups()),
        [&objectGroupName](Tmx::ObjectGroup * objGroup){
        return objGroup->GetName() == objectGroupName;
    });

    std::vector<ivec2> nodes;
    for (auto p : pathObjGroup->GetObjects())
    {
        nodes.push_back(ivec2(p->GetX(), p->GetY()));
        cout << p->GetX() << ' ' << p->GetY() << endl;
    }

    auto walls = dyb::parseWallXml( wallXmlFile);
    for (auto & w : walls)
        dyb::echoivec2(w.leftTop);
    return 0;
}
