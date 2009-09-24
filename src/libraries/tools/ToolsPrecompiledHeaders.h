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
    Compilation of the most often used header files in the tools library
@details
    Updated: 19. September 2009
    Total Files: 11
*/

#include "OrxonoxConfig.h"

///////////////////////////////////////////
/////          Stable Headers         /////
///////////////////////////////////////////

#include <cassert>  // 11
#include <cstring>  // 11
#include <fstream>  // 11
#include <iostream> // 11
#include <map>      // 11
#include <set>      // 11
#include <sstream>  // 11
#include <string>   // 11
#include <vector>   // 11
#include <cmath>    // 10
#include <deque>    // 10
#include <list>     // 10
#include <queue>    // 10

#include <OgreMath.h>        // 10
#include <OgreVector2.h>     // 10
#include <OgreVector3.h>     // 10
#include <OgreVector4.h>     // 10
#include <OgreQuaternion.h>  // 10
#include <OgreColourValue.h> // 10

#include "util/Debug.h"      // 9


// Just in case some header included windows.h
#undef min
#undef max
