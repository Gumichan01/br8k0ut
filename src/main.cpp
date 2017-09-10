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

#include "Game.hpp"

using namespace LX_Event;

namespace
{

const int GAME_WIDTH = 160;
const int GAME_HEIGHT = 144;
const std::string GAME_TITLE("Br8k0ut");
const std::string GAME_ICON("data/image/icon.ico");

}

int main(int argc, char** argv)
{
    LX_EventHandler ev;

    if(!LX_Init())
    {
        LX_Log::log("Cannot load the library: %s",LX_GetError());
        return -1;
    }

    //LX_Log::setDebugMode();     /// Remove it in realease mode

    // Information about how to build the window
    LX_Win::LX_WindowInfo info;
    LX_Win::LX_loadWindowConfig(info);
    info.title = GAME_TITLE;
    info.w = GAME_WIDTH;
    info.h = GAME_HEIGHT;

    LX_Win::LX_Window w(info);
    w.setIcon(GAME_ICON);
    LX_Win::LX_WindowManager::getInstance()->addWindow(&w);
    LX_Device::mouseCursorDisplay(LX_Device::LX_MOUSE_HIDE);

    {
        Game g(w);
        g.play();
    }


    LX_Win::LX_WindowManager::getInstance()->clearWindows();
    LX_Quit();
    return 0;
}
