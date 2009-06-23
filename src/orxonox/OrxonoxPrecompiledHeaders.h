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
    Compilation of the most often used header files in the orxonox executable
*/

#include "OrxonoxPrereqs.h"

#include <cassert>
#include <deque>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

// Prevent headers from including winsock.h that messes with winsock2.h from ENet
#define WIN32_LEAN_AND_MEAN


#ifdef ORXONOX_COMPILER_MSVC

// Note: Numbers after the include specify the number of times included (2009-06-19)
// The sizes in MB was just to see how much the MSVC9 PCH file increases for Ogre includes
#include <OgreMaterial.h> // 33, 2.7MB
#include <OgreRenderable.h> // 32, 2.6MB
#include <OgreSingleton.h> // 28, 0.0MB
#include <OgreSceneNode.h> // 25, 1.1MB
#include <OgreTexture.h> // 25, 0.2MB (1.1MB)
#include <OgreMovableObject.h> // 22, 1.3MB
#include <OgreResourceGroupManager.h> // 21, 1.4MB
#include <OgreTechnique.h> // 18, 1.2MB
//#include <OgreSceneManager.h> // 16, 7.4MB

#endif


//#include <boost/preprocessor/cat.hpp> // 15
//#include <boost/shared_ptr.hpp> // 13
#include <LinearMath/btMotionState.h>
#include <tinyxml/ticpp.h>

// Just in case some header included windows.h
#undef min
#undef max

//--------- Orxonox files --------
//--------------------------------

//#include "util/Convert.h" // 24
#include "util/Debug.h"
#include "util/Exception.h"
#include "util/Math.h"
#include "util/OgreForwardRefs.h"
#include "util/SubString.h"

#include "core/BaseObject.h"
//#include "core/ConfigValueIncludes.h" // 19
//#include "core/ConsoleCommand.h" // 15
#include "core/Core.h"
#include "core/CoreIncludes.h"
#include "core/GameMode.h"
#include "core/XMLPort.h"

#include "network/synchronisable/Synchronisable.h"
//#include "network/ClientInformation.h" // 26
//#include "network/ClientConnectionListener.h" // 24
