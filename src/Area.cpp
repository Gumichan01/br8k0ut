
/*
*   Br8k0ut - Platform Video game
*   Copyright © 2017 Luxon Jean-Pierre
*
*   This program is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*   Luxon Jean-Pierre (Gumichan01)
*   website: https://gumichan01.github.io/
*   mail: luxon.jean.pierre@gmail.com
*/

#include "Area.hpp"

#include <LunatiX/LX_Graphics.hpp>
#include <LunatiX/LX_WindowManager.hpp>
#include <LunatiX/LX_Window.hpp>
#include <LunatiX/LX_Log.hpp>

#include <TMXParser.h>
#include <TSXParser.h>
#include <sstream>
#include <cstdlib>
#include <regex>


namespace
{
const std::string MAP_PATH("./data/map/");
const int SPRITE_W = 8;
const int SPRITE_H = 8;
}

using namespace LX_Graphics;
using namespace LX_Win;

Area::Area(unsigned int lvl): level_id(lvl)
{
    TMX::Parser tmx;

    LX_Log::log("TMX");
    switch(lvl)
    {
    case 1:
        LX_Log::log("TMX: open ./data/map/01.tmx");
        tmx.load((MAP_PATH + "01.tmx").c_str());
        break;

    default:
        break;
    }

    LX_Log::log("Map Version: %s", tmx.mapInfo.version.c_str());
    LX_Log::log("Tileset [ Source: %s ]", tmx.tilesetList[0].source.c_str());

    const auto it = tmx.tileLayer.begin();
    LX_Log::log("Tile Layer Data contents:");

    if(LX_Log::isDebugMode())
        std::cout << tmx.tileLayer[it->first].data.contents << std::endl;

    // Convert the map
    parseMap(tmx.tileLayer[it->first].data.contents);

    LX_Log::log("END TMX\n");

    LX_Log::log("TSX");
    LX_Log::log("TSX: open %s", (MAP_PATH + tmx.tilesetList[0].source).c_str());
    TSX::Parser tsx;
    tsx.load(std::string(MAP_PATH + tmx.tilesetList[0].source).c_str());

    LX_Log::log("Name: %s", tsx.tileset.name.c_str());
    LX_Window *win = LX_WindowManager::getInstance()->getWindow(1);

    for(TSX::Parser::Tile& tile: tsx.tileList)
    {
        LX_Log::log("Tile: %d - %s", tile.id, (MAP_PATH + tile.img.name).c_str());
        sprites.push_back(new LX_Sprite(MAP_PATH + tile.img.name, *win, LX_PIXELFORMAT_BGR24));
    }

    LX_Log::log("END TSX\n");

    int i = 0;

    for(auto& arr : gtiles)
    {
        int j = 0;

        for(size_t k = 0; k < arr.size(); ++k)
        {
            arr[j].rect.x = j * SPRITE_W;
            arr[j].rect.y = i * SPRITE_H;
            arr[j].rect.w = SPRITE_W;
            arr[j].rect.h = SPRITE_H;
            j++;
        }

        i++;
    }
}

void Area::parseMap(const std::string& map_string)
{
    const std::regex CSV_FORMAT("[[:digit:]]+", std::regex::extended);

    std::istringstream stream(map_string);
    std::string line;
    size_t acount = 0;

    while(std::getline(stream, line))
    {
        LX_Log::log("%s\n", line.c_str());
        std::regex_iterator<std::string::iterator> it(line.begin(), line.end(), CSV_FORMAT);
        std::regex_iterator<std::string::iterator> it_end;
        size_t j = 0;

        while(it != it_end)
        {
            gtiles[acount][j].id_tile = std::atoi(it->str().c_str());
            gtiles[acount][j].id_sprite = gtiles[acount][j].id_tile - 1;
            it++;
            j++;
        }
    }
}

void Area::draw()
{
    /// @todo draw the level
    /*for(size_t i = 0; i < GAME_HEIGHT; ++i)
    {

    }*/

}

Area::~Area()
{
    for(size_t i = 0; i < sprites.size(); ++i)
    {
        delete sprites[i];
    }

    sprites.clear();
}
