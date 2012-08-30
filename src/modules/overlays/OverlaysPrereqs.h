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
    Shared library macros, enums, constants and forward declarations for the overlays module
*/

#ifndef _OverlaysPrereqs_H__
#define _OverlaysPrereqs_H__

#include "OrxonoxConfig.h"
#include "OrxonoxPrereqs.h"

//-----------------------------------------------------------------------
// Shared library settings
//-----------------------------------------------------------------------

#if defined(ORXONOX_PLATFORM_WINDOWS) && !defined(OVERLAYS_STATIC_BUILD)
#  ifdef OVERLAYS_SHARED_BUILD
#    define _OverlaysExport __declspec(dllexport)
#  else
#    if defined( __MINGW32__ )
#      define _OverlaysExport
#    else
#      define _OverlaysExport __declspec(dllimport)
#    endif
#  endif
#  define _OverlaysPrivate
#elif defined (ORXONOX_GCC_VISIBILITY)
#  define _OverlaysExport  __attribute__ ((visibility("default")))
#  define _OverlaysPrivate __attribute__ ((visibility("hidden")))
#else
#  define _OverlaysExport
#  define _OverlaysPrivate
#endif

//-----------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------

namespace orxonox
{
    class FadeoutText;
    class GUIOverlay;
    class OverlayText;

    // debugging
    class DebugFPSText;
    class DebugRTRText;

    // hud
    class AnnounceMessage;
    class BarColour;
    class ChatOverlay;
    class DeathMessage;
    class GametypeFadingMessage;
    class GametypeStaticMessage;
    class GametypeStatus;
    class HUDBar;
    class HUDHealthBar;
    class HUDNavigation;
    class HUDRadar;
    class HUDSpeedBar;
    class HUDBoostBar;
    class HUDTimer;
    class KillMessage;
    class LastManStandingInfos;
    class PauseNotice;
    class TeamBaseMatchScore;

    // stats
    class CreateLines;
    class Scoreboard;
    class Stats;
}

#endif /* _OverlaysPrereqs_H__ */
