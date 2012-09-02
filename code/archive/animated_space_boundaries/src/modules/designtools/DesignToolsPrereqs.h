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
 *      Oliver Scheuss
 *   Co-authors:
 *      ...
 *
 */

/**
@file
@brief
    Shared library macros, enums, constants and forward declarations for the pong module
*/

#ifndef _DesignToolsPrereqs_H__
#define _DesignToolsPrereqs_H__

#include "OrxonoxConfig.h"
#include "OrxonoxPrereqs.h"

//-----------------------------------------------------------------------
// Shared library settings
//-----------------------------------------------------------------------

#if defined(ORXONOX_PLATFORM_WINDOWS) && !defined(DESIGNTOOLS_STATIC_BUILD)
#  ifdef DESIGNTOOLS_SHARED_BUILD
#    define _DesignToolsExport __declspec(dllexport)
#  else
#    if defined( __MINGW32__ )
#      define _DesignToolsExport
#    else
#      define _DesignToolsExport __declspec(dllimport)
#    endif
#  endif
#  define _DesignToolsPrivate
#elif defined (ORXONOX_GCC_VISIBILITY)
#  define _DesignToolsExport  __attribute__ ((visibility("default")))
#  define _DesignToolsPrivate __attribute__ ((visibility("hidden")))
#else
#  define _DesignToolsExport
#  define _DesignToolsPrivate
#endif

//-----------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------

namespace orxonox
{
}

#endif /* _DesignToolsPrereqs_H__ */
