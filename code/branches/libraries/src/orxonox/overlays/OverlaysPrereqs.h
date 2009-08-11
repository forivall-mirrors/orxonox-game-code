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
  @brief Contains all the necessary forward declarations for all classes and structs.
*/

#ifndef _OverlaysPrereqs_H__
#define _OverlaysPrereqs_H__

#include "OrxonoxConfig.h"

//-----------------------------------------------------------------------
// Shared library settings
//-----------------------------------------------------------------------
#if defined(ORXONOX_PLATFORM_WINDOWS) && !defined(ORXONOX_STATIC_BUILD)
#  ifdef OVERLAYS_SHARED_BUILD
#    define _OverlaysExport __declspec(dllexport)
#  else
#    if defined( __MINGW32__ )
#      define _OverlaysExport
#    else
#      define _OverlaysExport __declspec(dllimport)
#    endif
#  endif
#elif defined ( ORXONOX_GCC_VISIBILITY )
#  define _OverlaysExport  __attribute__ ((visibility("default")))
#else
#  define _OverlaysExport
#endif

//-----------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------

namespace orxonox
{
    class BarColour;
    class DebugFPSText;
    class DebugRTRText;
    class GUIOverlay;
    class HUDBar;
    class HUDNavigation;
    class HUDRadar;
    class HUDSpeedBar;
    class HUDHealthBar;
    class HUDTimer;
    class Notification;
    class NotificationListener;
    class NotificationManager;
    class NotificationOverlay;
    class NotificationQueue;
    class OrxonoxOverlay;
    class OverlayGroup;
    class OverlayText;
    class FadeoutText;
    class GametypeStatus;
    class AnnounceMessage;
    class KillMessage;
    class DeathMessage;

    class CreateLines;
    class Scoreboard;
    class Stats;
}

#endif /* _OverlaysPrereqs_H__ */
