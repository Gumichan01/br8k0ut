
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

void loadLevel(unsigned int lvl, TMX::Parser& tmx)
{
    switch(lvl)
    {
    case 1:
        LX_Log::log("TMX: open ./data/map/01.tmx");
        tmx.load((MAP_PATH + "01.tmx").c_str());
        break;

    case 2:
        LX_Log::log("TMX: open ./data/map/02.tmx");
        tmx.load((MAP_PATH + "02.tmx").c_str());
        break;

    case 3:
        LX_Log::log("TMX: open ./data/map/03.tmx");
        tmx.load((MAP_PATH + "03.tmx").c_str());
        break;


    case 4:
        LX_Log::log("TMX: open ./data/map/04.tmx");
        tmx.load((MAP_PATH + "04.tmx").c_str());
        break;


    case 5:
        LX_Log::log("TMX: open ./data/map/05.tmx");
        tmx.load((MAP_PATH + "05.tmx").c_str());
        break;

    case 6:
        LX_Log::log("TMX: open ./data/map/06.tmx");
        tmx.load((MAP_PATH + "06.tmx").c_str());
        break;

    case 7:
        LX_Log::log("TMX: open ./data/map/07.tmx");
        tmx.load((MAP_PATH + "07.tmx").c_str());
        break;

    case 8:
        LX_Log::log("TMX: open ./data/map/08.tmx");
        tmx.load((MAP_PATH + "08.tmx").c_str());
        break;

    case 9:
        tmx.load((MAP_PATH + "09.tmx").c_str());
        LX_Log::log("TMX: open ./data/map/09.tmx");
        break;

    case 10:
        LX_Log::log("TMX: open ./data/map/10.tmx");
        tmx.load((MAP_PATH + "10.tmx").c_str());
        break;

    case 11:
        LX_Log::log("TMX: open ./data/map/11.tmx");
        tmx.load((MAP_PATH + "11.tmx").c_str());
        break;

    case 12:
        LX_Log::log("TMX: open ./data/map/12.tmx");
        tmx.load((MAP_PATH + "12.tmx").c_str());
        break;

    case 13:
        LX_Log::log("TMX: open ./data/map/13.tmx");
        tmx.load((MAP_PATH + "13.tmx").c_str());
        break;

    case 14:
        LX_Log::log("TMX: open ./data/map/14.tmx");
        tmx.load((MAP_PATH + "14.tmx").c_str());
        break;

    case 15:
        LX_Log::log("TMX: open ./data/map/15.tmx");
        tmx.load((MAP_PATH + "15.tmx").c_str());
        break;

    case 16:
        LX_Log::log("TMX: open ./data/map/16.tmx");
        tmx.load((MAP_PATH + "16.tmx").c_str());
        break;

    default:
        break;
    }
}

std::vector<LX_Graphics::LX_BufferedImage*> bimages;

}

using namespace LX_Graphics;
using namespace LX_Win;

const std::string Area::TYPE_NONE = "none";
const std::string Area::TYPE_SOLID = "solid";
const std::string Area::TYPE_START ="start";
const std::string Area::TYPE_DEATH = "death";
const std::string Area::TYPE_SHOT = "shot";
const std::string Area::TYPE_EXIT = "exit";

Area::Area(unsigned int lvl): level_id(lvl)
{
    TMX::Parser tmx;

    LX_Log::log("TMX");
    loadLevel(lvl, tmx);

    LX_Log::log("Map Version: %s", tmx.mapInfo.version.c_str());
    LX_Log::log("Tileset [ Source: %s ]", tmx.tilesetList[0].source.c_str());

    const auto it = tmx.tileLayer.begin();
    LX_Log::log("Tile Layer Data contents:");

    if(LX_Log::isDebugMode())
        std::cout << tmx.tileLayer[it->first].data.contents << std::endl;

    LX_Log::log("END TMX\n");

    {
        LX_Log::log("TSX");
        LX_Log::log("TSX: open %s", (MAP_PATH + tmx.tilesetList[0].source).c_str());
        TSX::Parser tsx;
        tsx.load(std::string(MAP_PATH + tmx.tilesetList[0].source).c_str());
        LX_Log::log("Name: %s", tsx.tileset.name.c_str());

        for(TSX::Parser::Tile& tile: tsx.tileList)
        {
            LX_Log::log("Tile: %d - %s", tile.id, (MAP_PATH + tile.img.name).c_str());
            bimages.push_back(new LX_BufferedImage(MAP_PATH + tile.img.name, LX_PIXELFORMAT_RGB888));
            Type *t = new Type();
            *t = {tile.id + 1, tile.type};
            vtypes.push_back(t);
        }

        LX_Log::log("END TSX\n");
    }

    /// Convert the map
    parseMap(tmx.tileLayer[it->first].data.contents);

    for(size_t i = 0; i < Game::GAME_HEIGHT; ++i)
    {
        for(size_t j = 0; j < Game::GAME_WIDTH; ++j)
        {
            gtiles[i * Game::GAME_WIDTH + j].rect.x = j * SPRITE_W;
            gtiles[i * Game::GAME_WIDTH + j].rect.y = i * SPRITE_H;
            gtiles[i * Game::GAME_WIDTH + j].rect.w = SPRITE_W;
            gtiles[i * Game::GAME_WIDTH + j].rect.h = SPRITE_H;
        }
    }

    generateMap();
}

void Area::parseMap(const std::string& map_string)
{
    const std::regex CSV_FORMAT("[[:digit:]]+", std::regex::extended);

    std::istringstream stream(map_string);
    std::string line;
    size_t acount = 0;

    while(std::getline(stream, line))
    {
        if(line.empty() || line.length() == 1)
            continue;

        std::regex_iterator<std::string::iterator> it(line.begin(), line.end(), CSV_FORMAT);
        std::regex_iterator<std::string::iterator> it_end;
        size_t j = 0;

        while(it != it_end)
        {
            gtiles[acount * Game::GAME_WIDTH + j].id_tile = std::atoi(it->str().c_str());
            gtiles[acount * Game::GAME_WIDTH + j].id_sprite = gtiles[acount * Game::GAME_WIDTH + j].id_tile - 1;

            unsigned int tmp_id = gtiles[acount * Game::GAME_WIDTH + j].id_tile;
            auto itf = std::find_if(vtypes.begin(), vtypes.end(), [&tmp_id](const Type* ty)
            {
                return ty->id == tmp_id;
            });

            if(itf != vtypes.end())
                gtiles[acount * Game::GAME_WIDTH + j].type = (*itf)->label;
            else
                gtiles[acount * Game::GAME_WIDTH + j].type = TYPE_NONE;

            it++;
            j++;
        }

        acount++;
    }
}


void Area::generateMap()
{
    LX_Window *win = LX_WindowManager::getInstance()->getWindow(1);
    map_texture = new LX_Graphics::LX_StreamingTexture(*win);

    // map the sprites on a streming texture
    for(GTile& tile: gtiles)
    {
        map_texture->blit(*bimages[tile.id_sprite], tile.rect);
    }

    map_texture->update();

    for(size_t k = 0; k < bimages.size(); ++k)
    {
        delete bimages[k];
    }
    bimages.clear();
}

void Area::draw()
{
    map_texture->draw();
}


const LX_AABB Area::getStart() const
{
    bool found = false;
    LX_AABB aabb;

    auto it = std::find_if(gtiles.begin(), gtiles.end(), [](const GTile& gt)
    {
        return gt.type == TYPE_START;
    });

    if(it != gtiles.end())
    {
        aabb = it->rect;
        found = true;
    }

    if(found)
        return aabb;

    throw std::string("An area must have a start point");
}

void Area::getCanons(std::vector<LX_AABB>& v)
{
    for(GTile& tile: gtiles)
    {
        if(tile.type == TYPE_SHOT)
            v.push_back(tile.rect);
    }
}


Area::~Area()
{
    for(size_t j = 0; j < vtypes.size(); ++j)
    {
        delete vtypes[j];
    }

    vtypes.clear();
    delete map_texture;
}
