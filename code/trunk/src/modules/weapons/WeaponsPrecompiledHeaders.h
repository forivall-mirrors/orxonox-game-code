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
    Compilation of the most often used header files in the weapons module
@details
    Updated: 13. September 2009
    Total Files: 13
*/

#include "OrxonoxConfig.h"

///////////////////////////////////////////
/////          Stable Headers         /////
///////////////////////////////////////////

#include <LinearMath/btMotionState.h> // 10
#include "util/OgreForwardRefs.h"     // 10

///////////////////////////////////////////
/////       All Rebuild Headers       /////
///////////////////////////////////////////

#include "core/CoreIncludes.h" // 13
#include "core/BaseObject.h" // 13
#include "tools/Timer.h" // 13

///////////////////////////////////////////
/////      Not so Stable Headers      /////
///////////////////////////////////////////

#include "network/synchronisable/Synchronisable.h" // 10
//#include "worldentities/WorldEntity.h"      // 10
//#include "weapons/projectiles/Projectile.h" // 9


// Just in case some header included windows.h
#undef min
#undef max
