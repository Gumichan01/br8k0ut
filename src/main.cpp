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


#include <LunatiX/Lunatix.hpp>

using namespace LX_Event;

namespace
{

const int GAME_WIDTH = 160;
const int GAME_HEIGHT = 144;

}

int main(int argc, char** argv)
{
    LX_AABB position = {0,0,36,26};
    LX_EventHandler ev;
    bool go = true;

    if(!LX_Init())
    {
        LX_Log::log("Cannot load the library: %s",LX_GetError());
        return -1;
    }

    // Information about how to build the window
    LX_Win::LX_WindowInfo info;
    LX_Win::LX_loadWindowConfig(info);
    info.title = "Br8k0ut";
    info.w = GAME_WIDTH;
    info.h = GAME_HEIGHT;

    LX_Win::LX_Window w(info);
    LX_Graphics::LX_Sprite sprite("data/sprite-sheet.png",w);

    while(go)
    {
        while(ev.pollEvent())
        {
            switch(ev.getEventType())
            {
            case LX_EventType::LX_QUIT:
                go = false;
                break;
            default:
                break;
            }
        }

        w.clearWindow();
        sprite.draw(&position);
        w.update();
        LX_Timer::delay(33);
    }

    LX_Quit();
    return 0;
}
