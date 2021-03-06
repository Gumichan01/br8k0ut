
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

#ifndef LX_SYSTEM_INFO_H_INCLUDED
#define LX_SYSTEM_INFO_H_INCLUDED

/**
*   @file LX_SystemInfo.hpp
*   @brief The System information library
*   @author Luxon Jean-Pierre(Gumichan01)
*   @version 0.10
*/

#include <LunatiX/utils/utf8_string.hpp>
#include <SDL2/SDL_video.h>
#include <vector>

/**
*   @defgroup System System
*   @brief System module (File system, Multithreading, ...)
*/

/**
*   @ingroup System
*   @namespace LX_SystemInfo
*   @brief The System information namespace
*
*   This namespace provides information about the system
*   (Platform, CPU, RAM)
*/
namespace LX_SystemInfo
{

using LX_DisplayMode = std::vector<SDL_DisplayMode>;

/**
*   @fn const UTF8string getPlatform()
*
*   Return the name of the patform.
*
*   @return The name of the platform the program is running on.
*          Or a string beginning with "Unkownn" if the name
*          is not available.
*
*   @note The names returned by the function is one of
*          these following names :
*          - Windows
*          - Mac OS X
*          - Linux
*/
const UTF8string getPlatform();

/**
*   @fn int getCPUCacheLineSize()
*   Get the L1 cache line size of the CPU
*   @return The L1 cache size of the CPU, in kilobytes (KB)
*/
int getCPUCacheLineSize();
/**
*   @fn int getCPUCount()
*   Get the CPU cores
*   @return The number of logical cores of the CPU
*
*   @note On CPU that include hyperthreading technology,
*   the value may be higher than the number of physical cores
*/
int getCPUCount();
/**
*   @fn int getSystemRAM()
*   Get the amount of Random Access Memory (RAM) in the system
*   @return The amount of RAM configured in the system in Megabytes (MB)
*/
int getSystemRAM();

/**
*   @fn void getAvailableDisplayModes(LX_DisplayMode& modes)
*
*   Get the list of available display modes on the screen
*
*   @param [in,out] modes the different modes of display to fill in
*
*   @note LX_DisplayMode is a struture that contains every available
*        display modes. In order to get these following modes, you should
*        use iterators.
*   @note Example:
*
*       LX_DisplayMode modes;
*       getAvailableDisplayModes(modes);
*
*       LX_Log::log("Display modes: ");
*       for(auto it = modes.begin(); it != modes.end(); it++)
*       {
*           LX_Log::log("%d × %d @ ~%d Hz", it->w, it->h, it->refresh_rate);
*       }
*.
*/
void getAvailableDisplayModes(LX_DisplayMode& modes);

}

#endif // LX_SYSTEM_INFO_H_INCLUDED
