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
    Compilation of the most often used header files in the orxonox library
@details
    Updated: 13. September 2009
    Total Files: 88
*/

#include "OrxonoxConfig.h"

///////////////////////////////////////////
/////          Stable Headers         /////
///////////////////////////////////////////

#include <cassert>  // 87
#include <cstring>  // 87
#include <fstream>  // 87
#include <iostream> // 87
#include <map>      // 87
#include <set>      // 87
#include <sstream>  // 87
#include <string>   // 87
#include <vector>   // 87
#include <list>     // 86
#include <ctime>    // 82
#include <cmath>    // 81
#include <deque>    // 81
#include <queue>    // 81

#include <OgreMath.h>        // 81
#include <OgreVector2.h>     // 81
#include <OgreVector3.h>     // 81
#include <OgreVector4.h>     // 81
#include <OgreQuaternion.h>  // 81
#include <OgreColourValue.h> // 81

#include "util/OgreForwardRefs.h"     // 67
#include <LinearMath/btMotionState.h> // 60
#include "util/SubString.h" // 55
#include <tinyxml/ticpp.h>  // 41

//#include <OgreRenderable.h> // 14, 5.1MB
//#include <OgreSceneNode.h>  // 13, 1MB
//#include <OgreResourceGroupManager.h> // 12, 1.5MB
//#include <OgreTexture.h>       // 11, 0.6MB
//#include <OgreMovableObject.h> // 10, 1.6MB
//#include <OgreSceneManager.h>  // 9, 8.7MB

///////////////////////////////////////////
/////       All Rebuild Headers       /////
///////////////////////////////////////////

#include "core/BaseObject.h"   // 82
#include "core/CoreIncludes.h" // 80

///////////////////////////////////////////
/////      Not so Stable Headers      /////
///////////////////////////////////////////

#include "network/synchronisable/Synchronisable.h" // 70
#include "util/MultiType.h"    // 65
#include "core/Executor.h"     // 55
//#include "core/XMLPort.h"     // 41


// Just in case some header included windows.h
#undef min
#undef max
