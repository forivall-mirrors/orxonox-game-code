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
    Shared library macros, enums, constants and forward declarations for the objects module
*/

#ifndef _ObjectsPrereqs_H__
#define _ObjectsPrereqs_H__

#include "OrxonoxConfig.h"
#include "OrxonoxPrereqs.h"

//-----------------------------------------------------------------------
// Shared library settings
//-----------------------------------------------------------------------

#if defined(ORXONOX_PLATFORM_WINDOWS) && !defined(OBJECTS_STATIC_BUILD)
#  ifdef OBJECTS_SHARED_BUILD
#    define _ObjectsExport __declspec(dllexport)
#  else
#    if defined( __MINGW32__ )
#      define _ObjectsExport
#    else
#      define _ObjectsExport __declspec(dllimport)
#    endif
#  endif
#  define _ObjectsPrivate
#elif defined (ORXONOX_GCC_VISIBILITY)
#  define _ObjectsExport  __attribute__ ((visibility("default")))
#  define _ObjectsPrivate __attribute__ ((visibility("hidden")))
#else
#  define _ObjectsExport
#  define _ObjectsPrivate
#endif

//-----------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------

namespace orxonox
{
    class Attacher;
    class ForceField;
    class Planet;
    class Script;
    class SpaceBoundaries;

    // collisionshapes
    class BoxCollisionShape;
    class ConeCollisionShape;
    class PlaneCollisionShape;
    class SphereCollisionShape;

    // eventsystem
    class EventDispatcher;
    class EventFilter;
    class EventListener;
    class EventName;
    class EventTarget;

    // triggers
    class CheckPoint;
    class DistanceMultiTrigger;
    class DistanceTrigger;
    class DistanceTriggerBeacon;
    class EventMultiTrigger;
    class EventTrigger;
    class MultiTrigger;
    class MultiTriggerContainer;
    class Trigger;
    class TriggerBase;
}

#endif /* _ObjectsPrereqs_H__ */
