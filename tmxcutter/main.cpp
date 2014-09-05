#include "../TmxParser/Tmx.h"
#include <cstdio>
#include <memory>
#include <string>
#include <algorithm>
#include <iostream>
#include <vector>
#include "debug.h"

using std::shared_ptr;
using std::string;
using std::vector;
using std::cout;
using std::endl;

// The first argument is tmx file, the second one is layer name
int main(int args, char * argv[])
{
    //const string tmxFile = argv[0];
    //const string layerName = argv[1];
    const string tmxFile = "forest.tmx";
    const string layerName = "meta";
    const string wallPropertyName = "wall";
    const string wallPropertyValue = "true";

    shared_ptr<Tmx::Map> map(new Tmx::Map());
    map->ParseFile(tmxFile);
    if (map->HasError()) {
        printf("error code: %d\n", map->GetErrorCode());
        printf("error text: %s\n", map->GetErrorText().c_str());
        return map->GetErrorCode();
    }

    // find wall tile ids
    vector<Tmx::Tile*> wallTiles;
    vector<unsigned int> wallTileIDs;
    for (Tmx::Tileset * tileset : map->GetTilesets())
    {
        for (Tmx::Tile * tile : tileset->GetTiles())
        {
            if (tile->GetProperties().HasProperty(wallPropertyName))
                wallTiles.push_back(tile);
        }
    }
    for (auto tile : wallTiles)
        wallTileIDs.push_back(tile->GetId());

    // find layer
    auto layers = map->GetLayers();
    Tmx::Layer * layer = *std::find_if(begin(layers), end(layers), [&layerName](Tmx::Layer * layer){
        return layer->GetName() == layerName;
    });

    // output
    for (int y = 0; y < layer->GetHeight(); ++y)
    {
        for (int x = 0; x < layer->GetWidth(); ++x)
        {
            unsigned int id = layer->GetTileId(x, y);
            if (find(begin(wallTileIDs), end(wallTileIDs), id) != end(wallTileIDs))
                cout << '0';
            else cout << '.';
        }
        cout << endl;
    }

    return 0;
}
