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
    Compilation of the most often used header files in the core library
@details
    Updated: 13. September 2009
    Total Files: 56
*/

#include "OrxonoxConfig.h"

///////////////////////////////////////////
/////          Stable Headers         /////
///////////////////////////////////////////

#include <string>   // 54
#include <vector>   // 54
#include <cassert>  // 53
#include <fstream>  // 53
#include <iostream> // 53
#include <map>      // 53
#include <sstream>  // 53
#include <set>      // 50

#include "util/Debug.h" // 48

#include <deque>    // 43
#include <list>     // 44
#include <cmath>    // 42

#include <loki/ScopeGuard.h> // 38

#include <OgreMath.h>        // 36
#include <OgreVector2.h>     // 36
#include <OgreVector3.h>     // 36
#include <OgreVector4.h>     // 36
#include <OgreQuaternion.h>  // 36
#include <OgreColourValue.h> // 36
#include <boost/preprocessor/cat.hpp> // 27
#include <boost/shared_ptr.hpp> // 21

#ifdef ORXONOX_COMPILER_MSVC

#include <ois/OISKeyboard.h> // 15
#include <ois/OISMouse.h>    // 15
#include <ois/OISJoyStick.h> // 15

#include "util/SubString.h"  // 14

#include <boost/scoped_ptr.hpp> // 13
#include <stack> // 12

#endif /*ORXONOX_COMPILER_MSVC */


// Just in case some header included windows.h
#undef min
#undef max
