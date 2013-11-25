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
    Shared library macros, enums, constants and forward declarations for the Invader module
*/

#ifndef _InvaderPrereqs_H__
#define _InvaderPrereqs_H__

#include "OrxonoxConfig.h"
#include "OrxonoxPrereqs.h"

//-----------------------------------------------------------------------
// Shared library settings
//-----------------------------------------------------------------------

#if defined(ORXONOX_PLATFORM_WINDOWS) && !defined(Invader_STATIC_BUILD)
#  ifdef Invader_SHARED_BUILD
#    define _InvaderExport __declspec(dllexport)
#  else
#    if defined( __MINGW32__ )
#      define _InvaderExport
#    else
#      define _InvaderExport __declspec(dllimport)
#    endif
#  endif
#  define _InvaderPrivate
#elif defined (ORXONOX_GCC_VISIBILITY)
#  define _InvaderExport  __attribute__ ((visibility("default")))
#  define _InvaderPrivate __attribute__ ((visibility("hidden")))
#else
#  define _InvaderExport
#  define _InvaderPrivate
#endif

//-----------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------

namespace orxonox
{
    class Invader;
    class InvaderCenterPoint;
    class InvaderShip;
    class InvaderEnemy;
    class InvaderWeapon;
    class InvaderHUDinfo;
}

#endif /* _InvaderPrereqs_H__ */
