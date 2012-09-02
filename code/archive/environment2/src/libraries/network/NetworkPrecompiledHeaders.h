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
    Compilation of the most often used header files in the network library
@details
    Updated: 13. September 2009
    Total Files: 28
*/

#include "OrxonoxConfig.h"

///////////////////////////////////////////
/////          Stable Headers         /////
///////////////////////////////////////////

#include <cstring>  // 26
#include <cassert>  // 24
#include <string>   // 23
#include <vector>   // 23
#include <map>      // 23
#include <fstream>  // 22
#include <iostream> // 22
#include <sstream>  // 22
#include <set>      // 20

#include "util/Debug.h"      // 20
#include <loki/TypeTraits.h> // 18

#ifdef ORXONOX_COMPILER_MSVC

#include <deque>    // 17
#include <queue>    // 17
#include <list>     // 16
#include <cmath>    // 15

#include <OgreMath.h>        // 15
#include <OgreVector2.h>     // 15
#include <OgreVector3.h>     // 15
#include <OgreVector4.h>     // 15
#include <OgreQuaternion.h>  // 15
#include <OgreColourValue.h> // 15

#define WIN32_LEAN_AND_MEAN
#include <enet/enet.h> // 6, 3.1MB

#include "util/mbool.h" // 11

#endif /*ORXONOX_COMPILER_MSVC */


// Just in case some header included windows.h
#undef min
#undef max
