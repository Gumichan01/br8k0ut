
/*
*   Br8k0ut - Platform Video game
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

#include "Framerate.hpp"

#include <LunatiX/LX_Log.hpp>
#include <LunatiX/LX_Timer.hpp>


namespace Framerate
{
const float SCREEN_FPS = 59.7f;
const float FRAME_DELAY = (1000.0f / SCREEN_FPS) + 1.0f;
const uint32_t SECOND = 1000;


void cycle()
{
    static uint32_t previous_time = 0;
    static int n = 0;
    n += 1;

    if((LX_Timer::getTicks() - previous_time) >= SECOND)
    {
        int fps = n;
        n = 0;
        previous_time = LX_Timer::getTicks();

        LX_Log::logDebug(LX_Log::LX_LOG_APPLICATION, "FPS: %d\n", fps);
    }
}

void regulate()
{
    static uint32_t prev_time = LX_Timer::getTicks();
    uint32_t ticks = (LX_Timer::getTicks() - prev_time);
    uint32_t frame_delay = static_cast<uint32_t>(FRAME_DELAY);

    if(ticks < frame_delay)
        LX_Timer::delay(static_cast<uint32_t>(frame_delay) - ticks);

    prev_time = LX_Timer::getTicks();
}

}
