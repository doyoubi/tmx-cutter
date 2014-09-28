#include <string>
#include <memory>
#include <fstream>
#include <sstream>

#include "../tinyxml2/tinyxml2.h"

#include "../dyb/debug.h"
#include "tmxWall.h"

namespace dyb
{
    using std::cout;
    using std::endl;
    using std::unique_ptr;
    using std::stoi;
    using std::ofstream;
    using std::tuple;

    tuple<vector<WallRect>, ivec2> parseWallXml(const string & wallXmlFile)
    {
        tinyxml2::XMLDocument doc;
        // doc.LoadFile does not work, it can't find the file
        std::ifstream file(wallXmlFile);
        if (!file.is_open())
        {
            cout << "can't load file: " << wallXmlFile << endl;
            exit(1);
        }
        std::stringstream ss;
        ss << file.rdbuf();
        if (doc.Parse(ss.str().c_str()) != tinyxml2::XML_NO_ERROR)
        {
            cout << "can't parse xml file: " << wallXmlFile << endl;
            doc.PrintError();
            exit(1);
        }
        vector<WallRect> ret;
        tinyxml2::XMLElement * root = doc.FirstChildElement("wall");
        DEBUGCHECK(root != nullptr, "can't find wall element in xml file. wall xml file should be generated by tmxcutter");
        ivec2 mapSizeInPixel;
        {
            int mapWidth = root->IntAttribute("mapWidth");
            int mapHeight = root->IntAttribute("mapHeight");
            int tileWidth = root->IntAttribute("tileWidth");
            int tileHeight = root->IntAttribute("tileHeight");
            DEBUGCHECK(mapWidth > 0 && mapHeight > 0 && tileWidth > 0 && tileHeight,
                "invalid map or tile size in xml attribute");
            mapSizeInPixel = ivec2(mapWidth * tileWidth, mapHeight * tileHeight);
        }
        for (tinyxml2::XMLNode * rect = root->FirstChild(); rect != nullptr; rect = rect->NextSibling())
        {
            ivec2 leftTop, size; // measured in pixel in tile map coordinate, not gl view port coordinate
            auto leftTopX = rect->FirstChildElement("leftTopX");
            auto leftTopY = rect->FirstChildElement("leftTopY");
            auto rightBottomX = rect->FirstChildElement("rightBottomX");
            auto rightBottomY = rect->FirstChildElement("rightBottomY");
            DEBUGCHECK(leftTopX && leftTopY && rightBottomX && rightBottomY,
                "invalid wall xml document. Can't find leftTopX or leftTopY or rightBottomX, rightBottomY");
            WallRect wallRect;
            try{
                wallRect.leftTop.x = std::stoi(leftTopX->GetText());
                wallRect.leftTop.y = std::stoi(leftTopY->GetText());
                wallRect.rightBottom.x = std::stoi(rightBottomX->GetText());
                wallRect.rightBottom.y = std::stoi(rightBottomY->GetText());
            }
            catch (const std::invalid_argument & e)
            {
                cout << e.what() << endl;
                cout << "can't parse string in " + wallXmlFile + " to int" << endl;
                cout << "please ensure text represent valid integer:" << endl;
                cout << "leftTopX: " << leftTopX->GetText() << endl
                    << "leftTopY: " << leftTopY->GetText() << endl
                    << "rightBottomX: " << rightBottomX->GetText() << endl
                    << "rightBottomY: " << rightBottomY->GetText() << endl;
                exit(1);
            }
            ret.push_back(wallRect);
        }
        return std::make_tuple(ret, mapSizeInPixel);
    }
}