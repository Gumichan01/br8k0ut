
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

#ifndef LX_OPENGL_H_INCLUDED
#define LX_OPENGL_H_INCLUDED


/**
*   @file LX_OpenGL.hpp
*   @brief The LunatiX/OpenGL interface
*   @author Luxon Jean-Pierre(Gumichan01)
*   @version 0.10
*/

#include <LunatiX/utils/utf8_string.hpp>
#include <SDL2/SDL_video.h>

/**
*   @ingroup Graphics
*   @namespace LX_Graphics
*   @brief The graphics namespace
*
*   It handlles rendering and operations (images, texture streaming,...)
*
*   @warning In order to use this namespace, the *video* flag
*   in the configuration file must be set to 1, otherwise the behaviour of
*   the library is undefined.
*/
namespace LX_Graphics
{

/**
*   @namespace LX_Graphics::LX_OpenGL
*   @brief The OpenGL interface namespace
*
*   It provides functions for using OpenGL with LunatiX
*
*   @warning In order to use this namespace, the *opengl* flag
*   in the configuration file must be set to 1, otherwise the behaviour of
*   the library is undefined.
*
*   @sa LX_Graphics
*/
namespace LX_OpenGL
{

using LX_GLattr = SDL_GLattr;
using LX_GLContext = SDL_GLContext;

const int LX_GL_NO_VSYNC        =  0;   /**< No VSync                   */
const int LX_GL_VSYNC           =  1;   /**< VSync                      */
const int LX_GL_NOT_SUPPORTED   = -1;   /**< Operation not supported    */
const int LX_GL_TEARING         = -1;   /**< Late swap Tearing          */

/**
*   @fn bool extensionSupported(std::string extension)
*
*   Check if an OpenGL extension is supported for the current context
*
*   @param [in] extension The name of the extension to check
*   @return TRUE if the extension is supported, FALSE otherwise
*/
bool extensionSupported(std::string extension);

/**
*   @fn bool loadDefaultLibrary()
*
*   Load the default OpenGL library
*
*   @return TRUE on success or FALSE on FAILURE.
*          Call LX_GetError() for more information
*
*   @note See ::loadLibrary
*
*/
bool loadDefaultLibrary();
/**
*   @fn bool loadLibrary(std::string path)
*
*   Load an OpenGL library
*
*   @param [in] path The platform dependent library name
*   @return TRUE on success or FALSE on FAILURE.
*          Call LX_GetError() for more information
*
*   @note 1 - This function should be called after initializing the video subsystem,
*            but before creating any OpenGL windows.
*   @note 2 - If no OpenGL library is loaded, the default library is loaded
*            upon creation of the first OpenGL window.
*   @note 3 - If you do this, you need to retrieve all of the OpenGL functions
*            used in your program from the dynamic library using *getProcAddress()*
*/
bool loadLibrary(std::string path);
/**
*   @fn void UnloadLibrary()
*
*   Unload the OpenGL library previously loaded by
*   *::loadDefaultLibrary()* or ::loadLibrary()*
*
*   @return TRUE on success or FALSE on FAILURE.
*          Call LX_GetError() for more information
*/
void UnloadLibrary();

/**
*   @fn template<typename T> T getProcAddress(const std::string proc);
*
*   Get an OpenGL function by name.
*
*   @param [in] proc The name of the function to load
*
*   @return A pointer to the OpenGL function on success,
*          NULL if the function is not found.
*
*   @note 1 - You have to specify the appropriate signature of the function
*   you want to get.
*   With SDL2, you should write a code like this:
*
*      typedef void (APIENTRY * GL_ActiveTextureARB_Func)(unsigned int);
*      GL_ActiveTextureARB_Func glActiveTextureARB_ptr = 0;
*      glActiveTextureARB_ptr = (GL_ActiveTextureARB_Func) SDL_GL_GetProcAddress("glActiveTextureARB");
*   Now, with LunatiX, you should do:
*
*      typedef void (APIENTRY * GL_ActiveTextureARB_Func)(unsigned int);
*      GL_ActiveTextureARB_Func glActiveTextureARB_ptr = 0;
*      glActiveTextureARB_ptr = LX_Graphics::LX_OpenGL::getProcAddress<GL_ActiveTextureARB_Func>("glActiveTextureARB");
*
*   @note 2 - It is very important to set the type parameter of *getProcAddress* → *getProcAddress<**type**>()* .
*      Otherwise, the code will not compile.
*
*   @note 3 - OpenGL function pointers must be declared **APIENTRY** as in the example code.
*   This will ensure the proper calling convention is followed on platforms
*   where this matters (Win32) thereby avoiding stack corruption.
*/
template<typename T>
T getProcAddress(const std::string proc);


/**
*   @fn LX_GLContext getCurrentContext()
*
*   Get the current context
*
*   @return The current active OpenGL context or NULL on failure.
*          Call LX_GetError() for more information
*/
LX_GLContext getCurrentContext();

/**
*   @fn int getSwapInterval()
*
*   Get the swap interval for the current OpenGL context
*
*   @return
*          - LX_GL_NO_VSYNC if there is no Vertical synchronization
*          - LX_GL_VSYNC if the swap is synchonized with the vertical retrace
*          - LX_GL_NOT_SUPPORTED if the swap interval is not supported.
*            Call LX_GetError() for more information
*
*   @sa setSwapInterval
*/
int getSwapInterval();
/**
*   @fn bool setSwapInterval(int interval)
*
*   Set the swap interval for the current OpenGL context
*
*   @param [in] interval One of the following values:
*          - ::LX_GL_NO_VSYNC : for immediate updates
*          - ::LX_GL_VSYNC    : for Vertical Synchronization (VSync)
*          - ::LX_GL_TEARING  : for late swap tearing
*
*   @return TRUE on success. FALSE if the setting is not supported.
*          Call LX_GetError() for more information
*
*   @note Some systems allow specifying -1 (LX_GL_TEARING)
*        for the interval in order to enable late swap tearing.
*        Late swap tearing (LX_GL_TEARING) works the same as vsync (LX_GL_VSYNC),
*        but if you've already missed the vertical retrace for a given frame,
*        it swaps buffers immediately, which might be less jarring
*        for the user during occasional framerate drops.
*        If application requests late swap tearing and the system does not support it,
*        this function will fail and return FALSE.
*        In such a case, you should probably retry the call with LX_GL_VSYNC for the interval.
*
*   @sa getSwapInterval
*/
bool setSwapInterval(int interval);

/**
*   @fn bool getAttribute(LX_GLattr attr, int& value)
*
*   Get the actual value for an attribute from the current context
*
*   @param [in] attr The [attribute](http://wiki.libsdl.org/SDL_GLattr) to query
*   @param [out] value The reference that will be filled in with
*               the current value of attr
*
*   @return TRUE on success, and value is set with the value odf the attribute,
*          FALSE otherwise.
*          Call LX_GetError() for more information.
*
*   @note the value the attribute are exactly the same with the attribute
*   defined in SDL2 (as of SDL 2.0.3)
*
*   @sa setAttribute
*/
bool getAttribute(LX_GLattr attr, int& value);
/**
*   @fn bool setAttribute(LX_GLattr attr, int value)
*
*   Set an OpenGL attribute before te creation of a window
*
*   @param [in] attr The [attribute](http://wiki.libsdl.org/SDL_GLattr) to set
*   @param [in] value The desired value for the attribute
*
*   @return TRUE on success. FALSE otherwise.
*          Call LX_GetError() for more information
*
*   @note This function sets the OpenGL attribute *attr* to *value*.
*        The requested attributes should be set before creating an OpenGL window.
*        You should use *getAttribute()* to check the values
*        after creating the OpenGL context,
*        since the values obtained can differ from the requested ones.
*
*   @sa getAttribute
*/
bool setAttribute(LX_GLattr attr, int value);

}

}

#include <LunatiX/LX_OpenGL.tpp>

#endif // LX_OPENGL_H_INCLUDED
