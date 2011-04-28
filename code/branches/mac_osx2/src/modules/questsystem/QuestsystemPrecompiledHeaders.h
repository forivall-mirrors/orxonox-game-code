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
    Compilation of the most often used header files in the quest system module
@details
    Updated: 19. September 2009
    Total Files: 24
*/

#include "OrxonoxConfig.h"

///////////////////////////////////////////
/////          Stable Headers         /////
///////////////////////////////////////////

#include <ctime> // 18

#include <OgreMath.h>        // 16
#include <OgreVector2.h>     // 16
#include <OgreVector3.h>     // 16
#include <OgreVector4.h>     // 16
#include <OgreQuaternion.h>  // 16
#include <OgreColourValue.h> // 16

#include <tinyxml/ticpp.h>   // 14
#include "util/Singleton.h"  // 13

///////////////////////////////////////////
/////       All Rebuild Headers       /////
///////////////////////////////////////////

#include "core/BaseObject.h"   // 24
#include "core/CoreIncludes.h" // 23

///////////////////////////////////////////
/////      Not so Stable Headers      /////
///////////////////////////////////////////

#include "core/command/Executor.h"     // 15
//#include "core/XMLPort.h"    // 14


// Just in case some header included windows.h
#undef min
#undef max
