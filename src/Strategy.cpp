
/*
*   Target_Xplosion - A classic shoot'em up video game
*   Copyright © 2017  Luxon Jean-Pierre
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

#include "Strategy.hpp"
#include "Shooter.hpp"
#include "Game.hpp"

#include <LunatiX/LX_Hitbox.hpp>
#include <LunatiX/LX_Vector2D.hpp>
#include <LunatiX/LX_Timer.hpp>

#include <cmath>

#define iabs(x) (static_cast<int>(x > 0 ? x : -x))

namespace
{
const unsigned int SHOT_DELAY = 1000;
}


/** Strategy implementation */
Strategy::Strategy(Shooter *s) : target(s), reference_time(LX_Timer::getTicks())
{
    target = s;
    reference_time = LX_Timer::getTicks();
}

Strategy::~Strategy() {}


/**
    Shot Strategy
    Shoot, shoot, shoot!
    That is all!
*/
ShotStrategy::ShotStrategy(Shooter *s) : Strategy(s), shot_delay(SHOT_DELAY) {}


void ShotStrategy::setShotDelay(unsigned int delay)
{
    shot_delay = delay;
}


void ShotStrategy::proceed()
{
    if((LX_Timer::getTicks() - reference_time) > shot_delay)
    {
        target->fire();
        reference_time = LX_Timer::getTicks();
    }
}

