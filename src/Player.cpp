
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

const float MAX_SPEED = 4.0f;
const float STEP_UP   = 0.5f;
const float STEP_DOWN = 1.0f;

const float GRAVITY   = 2.98f;
const float JUMP      = -8.0f;
const float JUMP_STEP = 1.0f;

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
    static short last = 0;

    if(speed.vx > 0.0f)
    {
        sprite->draw(&position);
        last = 0;
    }
    else if (speed.vx < 0.0f)
    {
        sprite->draw(&position, 0.0f, LX_Graphics::LX_MIRROR_HORIZONTAL);
        last = LX_Graphics::LX_MIRROR_HORIZONTAL;
    }
    else
    {
        if(last == 0)
            sprite->draw(&position);
        else
            sprite->draw(&position, 0.0f, LX_Graphics::LX_MIRROR_HORIZONTAL);

    }
}


void Player::inputState()
{
    const uint8_t *KEYS = LX_Event::LX_EventHandler::getKeyboardState().state;

    if(KEYS[SDL_SCANCODE_LEFT])
    {
        slow = false;

        if(speed.vx > -MAX_SPEED)
            speed.vx -= STEP_UP;
        else
            speed.vx = -MAX_SPEED;
    }

    if(KEYS[SDL_SCANCODE_RIGHT])
    {
        slow = false;

        if(speed.vx < MAX_SPEED)
            speed.vx += STEP_UP;
        else
            speed.vx = MAX_SPEED;
    }

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
}


void Player::input(const LX_Event::LX_EventHandler& ev)
{
    if(ev.getEventType() == LX_Event::LX_EventType::LX_KEYUP)
    {
        if(ev.getKeyCode() == SDLK_LEFT || ev.getKeyCode() == SDLK_RIGHT)
            slow = true;

        else if(ev.getKeyCode() == SDLK_SPACE)
        {
            /// @todo dash
        }
    }
}


void Player::move()
{
    fpos += speed;
    fpos.toPixelUnit(position);
    //LX_Log::log("posm: %d %d", position.x, position.y);
}

bool Player::status(const Area& area)
{
    int x = -1, y = -1;
    bool found = false;
    const int jmin = position.x / TILE_W;
    const int imin = position.y / TILE_H;
    const int jmax = (position.x + position.w - 1) / TILE_W;
    const int imax = (position.y + position.h - 1) / TILE_H;

    for(int i = imin; i <= imax; ++i)
    {
        for(int j = jmin; j <= jmax; ++j)
        {
            const GTile& tile = area.gtiles[i][j];

            if(collisionRect(position, tile.rect) && tile.type != Area::TYPE_NONE)
            {
                x = i;
                y = j;
                found = true;
                break;
            }
        }

        if(found)
            break;
    }

    if(x == -1 && y == -1)
    {
        if((area.gtiles[imax + 1][jmin].type == Area::TYPE_NONE
                && area.gtiles[imax + 1][jmax].type == Area::TYPE_NONE)
                || (area.gtiles[imax + 1][jmin].type == Area::TYPE_DEATH
                    && area.gtiles[imax +1][jmax].type == Area::TYPE_DEATH))
            speed.vy = GRAVITY;


        return false;
    }

    if(area.gtiles[x][y].type == Area::TYPE_DEATH)
    {
        fpos = area.getStart();
        position = area.getStart();
        speed *= 0.0f;
        return false;
    }
    else if(area.gtiles[x][y].type == Area::TYPE_EXIT)
        return true;

    else if(area.gtiles[x][y].type == Area::TYPE_SOLID)
    {
        bool xmod = false;
        bool left = false;
        const GTile& tile = area.gtiles[x][y];

        /// horizontal collision
        if(speed.vx > 0.0f && tile.rect.x < (position.x + position.w))
        {
            speed.vx = 0.0f;

            if(tile.rect.y <= position.y && speed.vy >= 0.0f)
            {
                fpos.x = tile.rect.x - position.w;
                position.x = tile.rect.x - position.w;
                xmod = true;
            }
        }
        else if(speed.vx < 0.0f && tile.rect.x + tile.rect.w > position.x)
        {
            speed.vx = 0.0f;
            left = true;
            LX_Log::log("left");

            if(tile.rect.y <= position.y && speed.vy >= 0.0f)
            {
                fpos.x = tile.rect.x + position.w;
                position.x = tile.rect.x + position.w;
                xmod = true;
            }
        }

        /// vertical collision
        if(speed.vy > 0.0f && tile.rect.y < (position.y + position.h))
        {
            if(!xmod)
            {
                fpos.y = tile.rect.y - position.h;
                position.y = tile.rect.y - position.h;
                speed.vy = 0.0f;
            }

            if(left)
            {
                if(area.gtiles[imax][jmax].type == Area::TYPE_SOLID)
                {
                    fpos.y = area.gtiles[imax][jmax].rect.y - position.h;
                    position.y = area.gtiles[imax][jmax].rect.y - position.h;
                    speed.vy = 0.0f;
                }
            }
        }
        else if(speed.vy <= 0.0f && tile.rect.y + tile.rect.h > position.y)
        {
            if(!xmod)
            {
                fpos.y = tile.rect.y + tile.rect.h + 1;
                position.y = tile.rect.y + tile.rect.h + 1;
                speed.vy = -speed.vy;
            }
        }

        return false;
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








