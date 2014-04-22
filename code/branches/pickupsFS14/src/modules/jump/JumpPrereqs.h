/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
 *                    > www.orxonox.net <
 *
 *
 *   License notice:
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License
 *   as published by the Free Software Foundation; either version 2
 *   of the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 *   Author:
 *      Reto Grieder
 *   Co-authors:
 *      ...
 *
 */

/**
@file
@brief
    Shared library macros, enums, constants and forward declarations for the jump module
*/

#ifndef _JumpPrereqs_H__
#define _JumpPrereqs_H__

#include "OrxonoxConfig.h"
#include "OrxonoxPrereqs.h"

//-----------------------------------------------------------------------
// Shared library settings
//-----------------------------------------------------------------------

#if defined(ORXONOX_PLATFORM_WINDOWS) && !defined(PONG_STATIC_BUILD)
#  ifdef PONG_SHARED_BUILD
#    define _JumpExport __declspec(dllexport)
#  else
#    if defined( __MINGW32__ )
#      define _JumpExport
#    else
#      define _JumpExport __declspec(dllimport)
#    endif
#  endif
#  define _JumpPrivate
#elif defined (ORXONOX_GCC_VISIBILITY)
#  define _JumpExport  __attribute__ ((visibility("default")))
#  define _JumpPrivate __attribute__ ((visibility("hidden")))
#else
#  define _JumpExport
#  define _JumpPrivate
#endif

//-----------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------

namespace orxonox
{
    class Jump;
    class JumpPlatform;
    class JumpPlatformStatic;
    class JumpPlatformHMove;
    class JumpPlatformVMove;
    class JumpFigure;
    class JumpCenterpoint;
    class JumpScore;
}

#endif /* _JumpPrereqs_H__ */
