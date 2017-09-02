
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


#ifndef AREA_HPP_INCLUDED
#define AREA_HPP_INCLUDED

#include "Game.hpp"

#include <LunatiX/LX_Graphics.hpp>
#include <array>

class Area
{
    unsigned int level_id;
    std::vector<LX_Graphics::LX_Sprite*> sprites;
    std::array<std::array<int, Game::GAME_WIDTH>, Game::GAME_HEIGHT> map;

public:

Area(unsigned int lvl);
void draw();
~Area();

};

#endif // AREA_HPP_INCLUDED
