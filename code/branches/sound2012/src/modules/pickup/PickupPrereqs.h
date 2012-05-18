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
 *      Damian 'Mozork' Frick
 *   Co-authors:
 *      ...
 *
 */

/**
@file
@brief
    Shared library macros, enums, constants and forward declarations for the questsystem module
*/

#ifndef _PickupPrereqs_H__
#define _PickupPrereqs_H__

#include "OrxonoxConfig.h"
#include "OrxonoxPrereqs.h"

//-----------------------------------------------------------------------
// Shared library settings
//-----------------------------------------------------------------------

#if defined(ORXONOX_PLATFORM_WINDOWS) && !defined(PICKUP_STATIC_BUILD)
#  ifdef PICKUP_SHARED_BUILD
#    define _PickupExport __declspec(dllexport)
#  else
#    if defined( __MINGW32__ )
#      define _PickupExport
#    else
#      define _PickupExport __declspec(dllimport)
#    endif
#  endif
#  define _PickupPrivate
#elif defined (ORXONOX_GCC_VISIBILITY)
#  define _PickupExport  __attribute__ ((visibility("default")))
#  define _PickupPrivate __attribute__ ((visibility("hidden")))
#else
#  define _PickupExport
#  define _PickupPrivate
#endif

//-----------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------

namespace orxonox
{

    class CollectiblePickup;
    class DroppedPickup;
    class Pickup;
    class PickupCollection;
    class PickupCollectionIdentifier;
    class PickupManager;
    class PickupRepresentation;
    class PickupSpawner;

    //items
    class DronePickup;
    class HealthPickup;
    class InvisiblePickup;
    class MetaPickup;
    class SpeedPickup;
    class ShieldPickup;
    class ShrinkPickup;

}

#endif /* _PickupPrereqs_H__ */
