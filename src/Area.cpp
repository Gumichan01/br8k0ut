
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


namespace
{
const std::string MAP_PATH("./data/map/");
}

using namespace LX_Graphics;
using namespace LX_Win;

Area::Area(unsigned int lvl): level_id(lvl)
{
    /// @todo build the area
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
        /// @todo store the pair. id/name
    }

    LX_Log::log("END TSX\n");
}


void Area::draw()
{
    /// @todo draw the level
}

Area::~Area()
{
    for(size_t i = 0; i < sprites.size(); ++i)
    {
        delete sprites[i];
    }

    sprites.clear();
}
