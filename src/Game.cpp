
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
#include "Area.hpp"
#include "Player.hpp"
#include "Shooter.hpp"
#include "Framerate.hpp"

#include <LunatiX/LX_Window.hpp>
#include <LunatiX/LX_Timer.hpp>
#include <LunatiX/LX_Mixer.hpp>
#include <LunatiX/LX_Music.hpp>

namespace
{
const std::string MUSIC_PATH("data/audio/gumichan01-eastern_wind.ogg");
const unsigned short VOLUME = 75;
}

using namespace LX_Event;


Game::Game(LX_Win::LX_Window& w) : done(false), lvl_count(0),
    exit_status(false), player(nullptr), win(w), music(nullptr)
{
    for(unsigned int i = 1; i <= NB_LEVELS; ++i)
    {
        areas.push_back(new Area(i));
    }

    LX_Mixer::setOverallVolume(VOLUME);
    music = new LX_Mixer::LX_Music(MUSIC_PATH);
}


void Game::play()
{
    music->play(-1);
    while(lvl_count < NB_LEVELS && !exit_status)
    {
        player = new Player(areas[lvl_count]->getStart(), *areas[lvl_count]);
        loop();
        delete player;
        player = nullptr;
        lvl_count++;
    }
    music->stop();
    music->close();
}


void Game::loadShooters()
{
    std::vector<LX_AABB> boxes;
    areas[lvl_count]->getCanons(boxes);

    for(const LX_AABB& b : boxes)
    {
        shooters.push_back(new Shooter(*this, b));
    }
}

void Game::loop()
{
    done = false;
    loadShooters();

    while(!done)
    {
        if((done = input()) == true)
            continue;

        status();
        physics();
        display();
        Framerate::regulate();
        Framerate::cycle();
    }
    clean();
}


bool Game::input()
{
    static bool full = false;
    bool _done = false;

    while(ev.pollEvent())
    {
        switch(ev.getEventType())
        {
        case LX_EventType::LX_QUIT:
            _done = true;
            exit_status = true;
            break;

        case LX_EventType::LX_KEYUP:
            if(ev.getKeyCode() == SDLK_f)
            {
                win.toggleFullscreen(full ? LX_Win::LX_WINDOW_NO_FULLSCREEN : LX_Win::LX_WINDOW_FULLSCREEN);
                full = !full;
            }
            else
                player->input(ev);
            break;

        default:
            break;
        }
    }

    player->inputState();

    return _done;
}



void Game::physics()
{
    /// @todo handle collision detection player bullets
    done = player->status();
}


void Game::status()
{
    /// @todo update status (bullets)
    player->move();

    for(Shooter *shooter: shooters)
        shooter->strategy();
}

void Game::clean()
{
    /// @todo clean (necessary?)
    for(size_t i = 0; i < shooters.size(); ++i)
    {
        delete shooters[i];
    }
    shooters.clear();
}

void Game::display()
{
    win.clearWindow();
    areas[lvl_count]->draw();
    player->draw();
    win.update();
}


Game::~Game()
{
    for(size_t i = 0; i < areas.size(); ++i)
    {
        delete areas[i];
    }

    areas.clear();
    delete player;
    delete music;
}
