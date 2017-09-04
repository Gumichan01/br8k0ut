
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


#include "Player.hpp"
#include "Area.hpp"

#include <LunatiX/LX_Hitbox.hpp>
#include <LunatiX/LX_WindowManager.hpp>
#include <LunatiX/LX_Graphics.hpp>
#include <LunatiX/LX_Physics.hpp>
#include <LunatiX/LX_Log.hpp>


namespace
{
const FloatPosition DFPOS = {0.0f, 0.0f};
const std::string PLAYER_PATH("./data/image/player.png");

const float MAX_SPEED = 3.5f;
const float STEP_UP = 0.5f;
const float STEP_DOWN = 1.0f;

bool slow = true;
}

using namespace LX_Win;
using namespace LX_Physics;

/// Floating-point coordinates

FloatPosition::FloatPosition(): x(0.0f), y(0.0f) {}
FloatPosition::FloatPosition(float fx, float fy): x(fx), y(fy) {}
FloatPosition::FloatPosition(const FloatPosition& fp): x(fp.x), y(fp.y) {}
FloatPosition::FloatPosition(const LX_AABB& b): FloatPosition(b.x, b.y) {}

FloatPosition& FloatPosition::operator =(const FloatPosition& fp)
{
    x = fp.x;
    y = fp.y;
    return *this;
}

FloatPosition& FloatPosition::operator =(const LX_AABB& aabb)
{
    x = aabb.x;
    y = aabb.y;
    return *this;
}


FloatPosition& FloatPosition::operator +=(const LX_Physics::LX_Vector2D& v)
{
    x += v.vx;
    y += v.vy;
    return *this;
}

void FloatPosition::toPixelUnit(LX_AABB& aabb)
{
    aabb.x = static_cast<int>(x);
    aabb.y = static_cast<int>(y);
}


/// Player

Player::Player(const LX_AABB& pos): sprite(nullptr), fpos(DFPOS), position(pos)
{
    LX_Window *win = LX_WindowManager::getInstance()->getWindow(1);
    sprite = new LX_Graphics::LX_Sprite(PLAYER_PATH, *win, LX_PIXELFORMAT_RGB888);
    fpos = position;
    speed *= 0.0f;
}

void Player::draw()
{
    if(speed.vx >= 0.0f)
        sprite->draw(&position);
    else
        sprite->draw(&position, 0.0f, LX_Graphics::LX_MIRROR_HORIZONTAL);

}


void Player::input(const LX_Event::LX_EventHandler& ev)
{
    if(ev.getEventType() ==  LX_Event::LX_EventType::LX_KEYDOWN)
    {
        if(ev.getKeyCode() == SDLK_LEFT)
        {
            slow = false;

            if(speed.vx > -MAX_SPEED)
                speed.vx -= STEP_UP;
            else
                speed.vx = -MAX_SPEED;
        }

        else if(ev.getKeyCode() == SDLK_RIGHT)
        {
            slow = false;

            if(speed.vx < MAX_SPEED)
                speed.vx += STEP_UP;
            else
                speed.vx = MAX_SPEED;
        }
    }

    if(ev.getEventType() == LX_Event::LX_EventType::LX_KEYUP)
    {
        if(ev.getKeyCode() == SDLK_LEFT || ev.getKeyCode() == SDLK_RIGHT)
            slow = true;
    }

    if(ev.getKeyCode() == SDLK_SPACE)
        LX_Log::log("JUMP");

    if(slow)
    {
        if(speed.vx > 0.0f)
        {
            if(speed.vx - STEP_DOWN < 0.0f)
                speed.vx = 0.0f;
            else
                speed.vx -= STEP_DOWN;
        }
        else if(speed.vx < 0.0f)
        {
            if(speed.vx + STEP_DOWN > 0.0f)
                speed.vx = 0.0f;
            else
                speed.vx += STEP_DOWN;
        }
    }

    LX_Log::log("speed x: %f", speed.vx);
}


void Player::move()
{
    fpos += speed;
    fpos.toPixelUnit(position);
}

bool Player::collision(const Area& area)
{
    for(auto& arr : area.gtiles)
    {
        for(const GTile& tile: arr)
        {
            if(collisionRect(position, tile.rect))
            {
                if(tile.type == Area::TYPE_SOLID)
                {
                    if(speed.vy > 0.0f && tile.rect.y < (position.y + position.h))
                    {
                        position.y = tile.rect.y - position.h;
                    }
                    else if (speed.vy < 0.0f && tile.rect.y + tile.rect.h > position.y)
                    {
                        position.y = tile.rect.y + position.h;
                    }
                }
                else if(tile.type == Area::TYPE_DEATH)
                {
                    position.x = area.getStart().x;
                    position.y = area.getStart().y;
                    speed *= 0.0f;
                }
                else if(tile.type == Area::TYPE_EXIT)
                {
                    return true;
                }
            }
        }
    }

    return false;
}


const LX_AABB& Player::getPos()
{
    return position;
}


Player::~Player()
{
    delete sprite;
}








