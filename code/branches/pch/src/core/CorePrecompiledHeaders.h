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
*/

#include <cassert>
#include <cmath>
#include <cstdlib>
#include <cctype>
#include <climits>

#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <list>
#include <set>
#include <map>
#include <fstream>

#include <OgreVector2.h>
#include <OgreVector3.h>
#include <OgreVector4.h>
#include <OgreQuaternion.h>
#include <OgreColourValue.h>
#include <ois/OISKeyboard.h>
#include <ois/OISMouse.h>
#include <ois/OISJoyStick.h>
#include <tinyxml/ticpp.h>

// GCC may have problems with anonymous namespaces in certain situations
#if !(defined(__GNUC__) && (__GNUC__ * 100 + __GNUC_MINOR__ < 420))
#  include "util/Convert.h"
#endif
#include "util/Debug.h"
#include "util/Exception.h"
#include "util/Math.h"
#include "util/MultiType.h"
#include "util/String.h"
#include "util/SubString.h"

// A change would trigger a 80% Core rebuild anyway
#include "Identifier.h"
