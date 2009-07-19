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
    Compilation of the most often used header files in the core library for MSVC
*/

#include "CorePrereqs.h"

#include <cassert>
#include <fstream>
#include <iostream>
#include <list>
#include <locale>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <vector>


#ifdef ORXONOX_COMPILER_MSVC

#include <OgreMath.h>
#include <OgreVector2.h>
#include <OgreVector3.h>
#include <OgreVector4.h>
#include <OgreQuaternion.h>
#include <OgreColourValue.h>

#include <boost/shared_ptr.hpp> // 12
#include <boost/preprocessor/cat.hpp> // 12

// Just in case some header included windows.h
#undef min
#undef max

#endif /* ORXONOX_COMPILER_MSVC */


#include "util/Debug.h"
#include "util/Math.h"
#include "util/MultiType.h"
#include "util/SubString.h"


#ifdef ORXONOX_COMPILER_MSVC

// A change would trigger an 80% Core rebuild anyway
#include "Identifier.h"

#endif /*ORXONOX_COMPILER_MSVC */
