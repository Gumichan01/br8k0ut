
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

#include "Shooter.hpp"
#include "Strategy.hpp"

#include <LunatiX/LX_Log.hpp>

Shooter::Shooter(Game& g, const LX_AABB& rect) : game(g), pos_start(rect),
    shot_strat(nullptr)
{
    shot_strat = new ShotStrategy(this);
}


void Shooter::strategy()
{
    shot_strat->proceed();
}

void Shooter::fire()
{
    LX_Log::log("FIRE");
    game.acceptBullet(pos_start);
}

Shooter::~Shooter()
{
    delete shot_strat;
}
