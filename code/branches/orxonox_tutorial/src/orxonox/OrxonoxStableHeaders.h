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
 @brief Contains the frequently used header files of our own writing
 */

#ifndef _OrxonoxStableHeaders_H__
#define _OrxonoxStableHeaders_H__

#include "util/OrxonoxPlatform.h"

#if ORXONOX_COMPILER == ORXONOX_COMPILER_MSVC && !defined(ORXONOX_DISABLE_PCH) && 0

// including std headers here is useless since they're already precompiled

#ifndef WIN32_LEAN_AND_MEAN
// prevent Ogre from including winsock.h that messes with winsock2.h from enet
#  define WIN32_LEAN_AND_MEAN
#endif
#include <Ogre.h>
#include <CEGUI.h>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/thread.hpp>

#include "tinyxml/ticpp.h"
#include "tinyxml/tinyxml.h"

//Get around Windows hackery (windows.h is included by Ogre)
#if ORXONOX_PLATFORM == ORXONOX_PLATFORM_WIN32 
#  ifdef max
#    undef max
#  endif
#  ifdef min
#    undef min
#  endif
#endif

//----------- Our files ----------
//--------------------------------
// only include when not debugging so that we may find issues with missing headers quicker
#if defined(NDEBUG)

#include "util/Convert.h"
#include "util/Debug.h"
#include "util/Math.h"
#include "util/Multitype.h"
#include "util/OutputBuffer.h"
#include "util/OutputHandler.h"
#include "util/Sleep.h"
#include "util/String.h"
#include "util/SubString.h"

#include "core/BaseObject.h"
#include "core/ConsoleCommand.h"
#include "core/CoreIncludes.h"
#include "core/ConfigValueIncludes.h"
#include "core/CommandExecutor.h"
#include "core/Executor.h"
#include "core/XMLIncludes.h"
#include "core/XMLPort.h"

#include "network/Synchronisable.h"

//#include "tools/Mesh.h"
//#include "tools/Timer.h"
//#include "objects/Model.h"
//#include "objects/Tickable.h"
//#include "objects/WorldEntity.h"

#endif /* ifdef NDEBUG */

#endif /* ORXONOX_COMPILER == ORXONOX_COMPILER_MSVC && !defined(ORXONOX_DISABLE_PCH) */

#endif /* _OrxonoxStableHeaders_H__ */
