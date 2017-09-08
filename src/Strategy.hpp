
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

#ifndef STRATEGY_H_INCLUDED
#define STRATEGY_H_INCLUDED

class Shooter;

namespace LX_Physics
{
struct LX_Point;
}

class Strategy
{
protected:

    Shooter *target;
    unsigned int reference_time;

public:

    explicit Strategy(Shooter *s);

    virtual void proceed() = 0;
    virtual ~Strategy();

};

// Just shoot!
class ShotStrategy: public Strategy
{
protected:

    unsigned int shot_delay;

public:

    explicit ShotStrategy(Shooter *s);

    void setShotDelay(unsigned int delay);
    void proceed();

    ~ShotStrategy() = default;
};


#endif // STRATEGY_H_INCLUDED
