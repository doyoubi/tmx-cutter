#include <memory>
#include <string>
#include <algorithm>
#include <iostream>
#include <vector>

#include "../TmxParser/Tmx.h"
#include "../TmxParser/TinyXML/tinyxml.h"

#include "debug.h"
#include "array_2d.h"
#include "point2d.h"
#include "cut.h"
#include <fstream>

using std::shared_ptr;
using std::string;
using std::vector;
using std::cout;
using std::endl;

typedef dyb::point2d<int> ivec2;
typedef dyb::array2d<char> rect;

void printHelp()
{
    cout << "usage :" << endl;
    cout << "\t tmxcutter [tmx file] [layer name] [wall property name] [xml file name]" << endl<<endl;
    cout << "\t [layer name] specify the layer where your 'wall tile' locate in" << endl;
    cout << "\t Tiles with property [wall property name] will be seen as the 'wall tile' and will be grouped into rectangle" << endl;
    cout << "\t [xml file name] specify the output xml file you want to save" << endl;
}

// The first argument is tmx file, the second one is layer name
int main(int args, char * argv[])
{
    if (args != 5) 
    {
        printHelp();
        return 0;
    }
    const string tmxFile = argv[1];
    const string layerName = argv[2];
    const string wallPropertyName = argv[3];
    const string savedXmlFile = argv[4];

    // for debugging
    /*const string tmxFile = "forest.tmx";
    const string layerName = "meta";
    const string wallPropertyName = "wall";
    const string savedXmlFile = "output.xml";*/

    shared_ptr<Tmx::Map> map(new Tmx::Map());
    map->ParseFile(tmxFile);
    if (map->HasError())
    {
        cout << "error code : " << map->GetErrorCode() << endl;
        cout << "error text: " << map->GetErrorText() << endl;
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
    if (wallTiles.empty())
    {
        cout << "can't find tile with property :" << wallPropertyName << endl;
        return 1;
    }

    // find layer
    auto layers = map->GetLayers();
    auto layerIter = std::find_if(begin(layers), end(layers), [&layerName](Tmx::Layer * layer){
        return layer->GetName() == layerName;
    });
    if (layerIter == end(layers))
    {
        cout << "can't find layer named " << layerName << endl;
        return 1;
    }
    Tmx::Layer * layer = *layerIter;

    // construct map
    rect input(layer->GetWidth(), layer->GetHeight());
    for (int y = 0; y < layer->GetHeight(); ++y)
    {
        for (int x = 0; x < layer->GetWidth(); ++x)
        {
            unsigned int id = layer->GetTileId(x, y);
            // '0' means the tile is wall and sprite can't intersect with it while '.' means not
            // this two tag will be used in cut() function to cut the tile polygons to rectangular pieces
            if (find(begin(wallTileIDs), end(wallTileIDs), id) != end(wallTileIDs))
                input[x][y] = '0';
            else input[x][y] = '.';
        }
    }

    // cut tile polygons into rectangular pieces
    using dyb::TileRect;
    using dyb::PixelRect;
    vector<TileRect> tileRects = dyb::cut(input);

    vector<PixelRect> pixelrects;
    for (auto & tileRect : tileRects)
    {
        ivec2 leftTop, rightBottom;
        leftTop.x = tileRect.leftTop.x * map->GetTileWidth();
        leftTop.y = tileRect.leftTop.y * map->GetTileHeight();
        rightBottom.x = (tileRect.rightBottom.x + 1) * map->GetTileWidth() - 1;
        rightBottom.y = (tileRect.rightBottom.y + 1) * map->GetTileHeight() - 1;
        pixelrects.push_back({leftTop, rightBottom});
        // for debugging
        /*dyb::echoivec2(leftTop);
        dyb::echoivec2(rightBottom);
        cout << "--------------------------" << endl;*/
    }

    // write result to xml file
    TiXmlDocument doc;
    TiXmlDeclaration * decl = new TiXmlDeclaration("1.0", "", "");
    doc.LinkEndChild(decl);
    TiXmlElement * root = new TiXmlElement("wall");
    doc.LinkEndChild(root);
    for (auto & pixelRect : pixelrects)
    {
        using std::to_string;
        TiXmlElement * rect = new TiXmlElement("rect");
        root->LinkEndChild(rect);
        TiXmlElement * width = new TiXmlElement("width");
        TiXmlElement * height = new TiXmlElement("height");
        rect->LinkEndChild(width);
        rect->LinkEndChild(height);

        ivec2 delta = pixelRect.rightBottom - pixelRect.leftTop;
        width->LinkEndChild(new TiXmlText(to_string(delta.x + 1)));
        height->LinkEndChild(new TiXmlText(to_string(delta.y + 1)));

        TiXmlElement * leftTopX = new TiXmlElement("leftTopX");
        TiXmlElement * leftTopY = new TiXmlElement("leftTopY");
        TiXmlElement * rightBottomX = new TiXmlElement("rightBottomX");
        TiXmlElement * rightBottomY = new TiXmlElement("rightBottomY");
        rect->LinkEndChild(leftTopX);
        rect->LinkEndChild(leftTopY);
        rect->LinkEndChild(rightBottomX);
        rect->LinkEndChild(rightBottomY);
        leftTopX->LinkEndChild(new TiXmlText(to_string(pixelRect.leftTop.x)));
        leftTopY->LinkEndChild(new TiXmlText(to_string(pixelRect.leftTop.y)));
        rightBottomX->LinkEndChild(new TiXmlText(to_string(pixelRect.rightBottom.x)));
        rightBottomY->LinkEndChild(new TiXmlText(to_string(pixelRect.rightBottom.y)));
    }
    doc.SaveFile(savedXmlFile);

    return 0;
}
