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
    Compilation of the most often used header files in the objects module
@details
    Updated: 19. September 2009
    Total Files: 15
*/

#include "OrxonoxConfig.h"

///////////////////////////////////////////
/////          Stable Headers         /////
///////////////////////////////////////////

#include <LinearMath/btTransform.h> // 11

///////////////////////////////////////////
/////       All Rebuild Headers       /////
///////////////////////////////////////////

#include "core/BaseObject.h"   // 15
#include "core/CoreIncludes.h" // 15
#include "core/XMLPort.h"      // 13

///////////////////////////////////////////
/////      Not so Stable Headers      /////
///////////////////////////////////////////

#include "network/synchronisable/Synchronisable.h" // 11
//#include "orxonox/worldentities/StaticEntity.h" // 7


// Just in case some header included windows.h
#undef min
#undef max
