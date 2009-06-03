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

// including std headers here is useless since they're already precompiled

#ifndef WIN32_LEAN_AND_MEAN
// prevent Ogre from including winsock.h that messes with winsock2.h from enet
#  define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#  define NOMINMAX // required to stop windows.h screwing up std::min definition
#endif
#include <Ogre.h>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/static_assert.hpp>
#include <boost/filesystem.hpp>

#include "BulletCollision/CollisionShapes/btCollisionShape.h"
#include "BulletDynamics/Dynamics/btRigidBody.h"
#include "tinyxml/ticpp.h"
#include "tolua/tolua++.h"

//--------- Orxonox files --------
//--------------------------------

#include "util/Convert.h"
#include "util/Debug.h"
#include "util/Exception.h"
#include "util/Math.h"
#include "util/MathConvert.h"
#include "util/Multitype.h"
#include "util/Sleep.h"
#include "util/String.h"
#include "util/SubString.h"

#include "core/BaseObject.h"
#include "core/ClassTreeMask.h"
#include "core/Clock.h"
#include "core/ConsoleCommand.h"
#include "core/CoreIncludes.h"
#include "core/ConfigValueIncludes.h"
#include "core/CommandExecutor.h"
#include "core/CommandLine.h"
#include "core/Core.h"
#include "core/EventIncludes.h"
#include "core/Executor.h"
#include "core/Game.h"
#include "core/GameMode.h"
#include "core/GameState.h"
#include "core/ObjectList.h"
#include "core/Super.h"
#include "core/Template.h"
#include "core/XMLFile.h"
#include "core/XMLIncludes.h"
#include "core/XMLPort.h"
#include "core/input/SimpleInputState.h"
#include "core/input/InputManager.h"

#include "network/synchronisable/Synchronisable.h"
#include "network/ClientInformation.h"
