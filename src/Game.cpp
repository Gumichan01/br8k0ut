
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

#include "Game.hpp"

#include <LunatiX/LX_Window.hpp>
#include <LunatiX/LX_Timer.hpp>

using namespace LX_Event;

Game::Game(LX_Win::LX_Window& w) : win(w)
{

}


void Game::loop()
{
    bool done = false;
    while(!done)
    {
        if((done = input()) == true)
            continue;

        physics();
        status();
        clean();
        display();
    }
}


bool Game::input()
{
    bool done = false;
    while(ev.pollEvent())
    {
        switch(ev.getEventType())
        {
        case LX_EventType::LX_QUIT:
            done = true;
            break;
        default:
            break;
        }
    }

    return done;
}



void Game::physics()
{
    /// @todo handle collision detection, gravity
}


void Game::status()
{
    /// @todo update status (player, bullets)
}

void Game::clean()
{
    /// @todo clean (necessary?)
}

void Game::display()
{
    win.clearWindow();
    win.update();
    LX_Timer::delay(33);
}




Game::~Game()
{

}
