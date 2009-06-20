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
*/

#include "NetworkPrereqs.h"

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
#include <enet/enet.h>
#undef max
#undef min

// Too larg PCH file if you include this and only 10% faster
//#include <boost/thread/recursive_mutex.hpp>

#include "util/CRC32.h"
#include "util/Debug.h"
#include "util/Math.h"
#include "util/mbool.h"
#include "util/MultiType.h"
#include "util/String.h"

#include "core/Core.h"
#include "core/CoreIncludes.h"
#include "core/Functor.h"
#include "core/GameMode.h"

#endif /* ORXONOX_COMPILER_MSVC */
