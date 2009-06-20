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

#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>


#ifdef ORXONOX_COMPILER_MSVC

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef max
#undef min

#include <ois/OISKeyboard.h>
#include <ois/OISMouse.h>
#include <ois/OISJoyStick.h>
#include <tinyxml/ticpp.h>
// Included by both filesystem and thread but still relatively small
#include <boost/iterator/iterator_facade.hpp>

#endif /* ORXONOX_COMPILER_MSVC */


#include "util/Convert.h"
#include "util/Debug.h"
#include "util/Exception.h"
#include "util/Math.h"
#include "util/mbool.h"
#include "util/MultiType.h"
#include "util/OrxAssert.h"
#include "util/OrxEnum.h"
#include "util/String.h"
#include "util/SubString.h"


#ifdef ORXONOX_COMPILER_MSVC

// A change would trigger an 80% Core rebuild anyway
#include "Identifier.h"

#endif /*ORXONOX_COMPILER_MSVC */
