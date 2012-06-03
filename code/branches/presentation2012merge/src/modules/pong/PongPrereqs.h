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
    Shared library macros, enums, constants and forward declarations for the pong module
*/

#ifndef _PongPrereqs_H__
#define _PongPrereqs_H__

#include "OrxonoxConfig.h"
#include "OrxonoxPrereqs.h"

//-----------------------------------------------------------------------
// Shared library settings
//-----------------------------------------------------------------------

#if defined(ORXONOX_PLATFORM_WINDOWS) && !defined(PONG_STATIC_BUILD)
#  ifdef PONG_SHARED_BUILD
#    define _PongExport __declspec(dllexport)
#  else
#    if defined( __MINGW32__ )
#      define _PongExport
#    else
#      define _PongExport __declspec(dllimport)
#    endif
#  endif
#  define _PongPrivate
#elif defined (ORXONOX_GCC_VISIBILITY)
#  define _PongExport  __attribute__ ((visibility("default")))
#  define _PongPrivate __attribute__ ((visibility("hidden")))
#else
#  define _PongExport
#  define _PongPrivate
#endif

//-----------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------

namespace orxonox
{
    class Pong;
    class PongAI;
    class PongBall;
    class PongBat;
    class PongBot;
    class PongCenterpoint;
    class PongScore;
}

#endif /* _PongPrereqs_H__ */
