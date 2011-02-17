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
    Compilation of the most often used header files in the overlays module
@details
    Updated: 19. September 2009
    Total Files: 21
*/

#include "OrxonoxConfig.h"

///////////////////////////////////////////
/////          Stable Headers         /////
///////////////////////////////////////////

#include <tinyxml/ticpp.h> // 14
//#include <OgreOverlayelement.h> // 7, 7.2MB
#include <bullet/LinearMath/btMotionState.h> // 6
//#include <OgrePanelOverlayelement.h> // 5

///////////////////////////////////////////
/////       All Rebuild Headers       /////
///////////////////////////////////////////

#include "overlays/OrxonoxOverlay.h" // 21
#include "core/CoreIncludes.h"       // 20

///////////////////////////////////////////
/////      Not so Stable Headers      /////
///////////////////////////////////////////

#include "core/command/Executor.h"     // 13
//#include "network/synchronisable/Synchronisable.h" // 13
//#include "core/XMLPort.h"    // 9


// Just in case some header included windows.h
#undef min
#undef max
