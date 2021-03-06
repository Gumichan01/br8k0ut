
/*
*   Copyright © 2017 Luxon Jean-Pierre
*   https://gumichan01.github.io/
*
*   LunatiX is a free, SDL2-based library.
*   It can be used for open-source or commercial games thanks to the zlib/libpng license.
*
*   Luxon Jean-Pierre (Gumichan01)
*   luxon.jean.pierre@gmail.com
*/

#ifndef LX_AABB_H_INCLUDED
#define LX_AABB_H_INCLUDED

/**
*   @file LX_AABB.hpp
*   @brief The definition of LX_AABB
*   @author Luxon Jean-Pierre(Gumichan01)
*   @version 0.10
*/

// LX_AABB is just an alias of SDL_Rect
struct SDL_Rect;
/**
*   @ingroup Physics
*   @typedef LX_AABB
*   @brief The Axis-Aligned Bounding Box (AABB)
*
*   Structure:
*
*      int x, int y;   // Coordinates
*      int w, int h;   // Dimension
*
*   Very useful for collision detection, image positioning, ...
*/
using LX_AABB = SDL_Rect;

#include <SDL2/SDL_rect.h>

#endif  /* LX_AABB_H_INCLUDED */
