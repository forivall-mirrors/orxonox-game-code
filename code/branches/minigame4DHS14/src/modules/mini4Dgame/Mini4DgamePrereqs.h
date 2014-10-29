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

#ifndef _Mini4DgamePrereqs_H__
#define _Mini4DgamePrereqs_H__

#include "OrxonoxConfig.h"
#include "OrxonoxPrereqs.h"

//-----------------------------------------------------------------------
// Shared library settings
//-----------------------------------------------------------------------

#if defined(ORXONOX_PLATFORM_WINDOWS) && !defined(Mini4Dgame_STATIC_BUILD)
#  ifdef Mini4Dgame_SHARED_BUILD
#    define _Mini4DgameExport __declspec(dllexport)
#  else
#    if defined( __MINGW32__ )
#      define _Mini4DgameExport
#    else
#      define _Mini4DgameExport __declspec(dllimport)
#    endif
#  endif
#  define _Mini4DgamePrivate
#elif defined (ORXONOX_GCC_VISIBILITY)
#  define _Mini4DgameExport  __attribute__ ((visibility("default")))
#  define _Mini4DgamePrivate __attribute__ ((visibility("hidden")))
#else
#  define _Mini4DgameExport
#  define _Mini4DgamePrivate
#endif

//-----------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------

namespace orxonox
{
    class Mini4Dgame;
    class Mini4DgameCenterpoint;
}

#endif /* _PongPrereqs_H__ */
