#include <memory>
#include <string>
#include <iostream>
#include <algorithm>

#include "glm/vec2.hpp"
#include "../TmxParser/Tmx.h"

#include "tmxObjNode.h"

namespace dyb
{
    using std::cout;
    using std::endl;
    using std::string;
    using std::shared_ptr;
    using glm::ivec2;

    std::vector<ivec2> getTmxObjNode(const Tmx::Map & map, const string & objectGroupName)
    {
        Tmx::ObjectGroup * pathObjGroup = *std::find_if(
            begin(map.GetObjectGroups()), end(map.GetObjectGroups()),
            [&objectGroupName](Tmx::ObjectGroup * objGroup){
            return objGroup->GetName() == objectGroupName;
        });

        std::vector<ivec2> nodes;
        for (auto p : pathObjGroup->GetObjects())
        {
            nodes.push_back(ivec2(p->GetX(), p->GetY()));
        }
        return nodes;
    }
}