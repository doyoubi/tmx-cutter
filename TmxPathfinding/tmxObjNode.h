#ifndef DYB_TMX_OBJ_NODE
#define DYB_TMX_OBJ_NODE

#include <vector>
#include <string>
#include <memory>
#include "glm/vec2.hpp"

namespace dyb
{
    std::vector<glm::ivec2> getTmxObjNode(const Tmx::Map & map, const std::string & objectGroupName);
}

#endif