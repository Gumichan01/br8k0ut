
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

#ifndef GAME_HPP_INCLUDED
#define GAME_HPP_INCLUDED

#include <LunatiX/LX_Event.hpp>

#include <vector>

namespace LX_Win
{
class LX_Window;
}

namespace LX_Mixer
{
class LX_Music;
}

class Area;
class Player;

/// @todo store the start point

class Game
{
    static const unsigned int NB_LEVELS = 6;

    bool done;
    unsigned int lvl_count;
    bool exit_status;
    Player * player;
    std::vector<Area*> areas;
    //std::vector<Shooter*> shooter;

    LX_Win::LX_Window& win;
    LX_Mixer::LX_Music *music;
    LX_Event::LX_EventHandler ev;

    /// @todo Player, entity, shooter
    /// @todo Music

public:

    static const int GAME_WIDTH = 160;
    static const int GAME_HEIGHT = 144;

    Game(LX_Win::LX_Window& w);

    void play();
    void loop();
    // The game logic
    bool input();
    void physics();
    void status();
    void clean();
    void display();

    ~Game();
};

#endif // GAME_HPP_INCLUDED
