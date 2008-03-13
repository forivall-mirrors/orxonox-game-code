/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
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
 @file  OrxonoxStableHeaders.h
 @brief Contains the bigger (or smaller) header files in order to precompile them with MSVC
 */

#ifndef _OrxonoxStableHeaders_H__
#define _OrxonoxStableHeaders_H__

#include "OrxonoxPlatform.h"

#if ORXONOX_COMPILER == ORXONOX_COMPILER_MSVC

// including std headers here is useless since they're already precompiled

// not including the entire Ogre.h doesn't exceed the default heap size for pch
//#include <Ogre.h>
#include <OgreBillboardSet.h>
#include <OgreCamera.h>
#include <OgreConfigFile.h>
#include <OgreEntity.h>
#include <OgreException.h>
#include <OgreFrameListener.h>
#include <OgreLight.h>
#include <OgreOverlay.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OgreParticleEmitter.h>
#include <OgreParticleSystem.h>
#include <OgrePlatform.h>
#include <OgrePrerequisites.h>
#include <OgreRenderWindow.h>
#include <OgreRoot.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreStringConverter.h>
#include <OgreTextureManager.h>
#include <OgreViewport.h>

#include <OIS/OIS.h>

/**
* Some of the not so stable header files.
* But it's not very useful to include them anyway..
**/

//#include "audio/AudioManager.h"

//#include "core/CoreIncludes.h"
#include "core/BaseObject.h"
//#include "core/ArgReader.h"
#include "core/Error.h"

#include "network/Synchronisable.h"
//#include "network/Server.h"
//#include "network/Client.h"

#include "util/Math.h"
#include "util/Convert.h"
#include "util/Sleep.h"
#include "util/String2Number.h"
#include "util/Tokenizer.h"
#include "util/tinyxml/ticpp.h"
#include "util/tinyxml/tinyxml.h"

//#include "hud/HUD.h"
//#include "loader/LevelLoader.h"
//#include "objects/weapon/AmmunitionDump.h"
//#include "objects/weapon/BarrelGun.h"
//#include "objects/weapon/Bullet.h"
//#include "objects/weapon/BulletManager.h"
//#include "objects/weapon/WeaponStation.h"
//#include "objects/Ambient.h"
//#include "objects/Camera.h"
//#include "objects/Explosion.h"
//#include "objects/Fighter.h"
#include "objects/Model.h"
//#include "objects/NPC.h"
//#include "objects/Projectile.h"
//#include "objects/Skybox.h"
//#include "objects/SpaceShipSteeringObject.h"
#include "objects/Tickable.h"
#include "objects/WorldEntity.h"
//#include "particle/ParticleInterface.h"
//#include "tools/BillboardSet.h"
//#include "tools/Light.h"
//#include "tools/Mesh.h"
//#include "tools/Timer.h"
//#include "GraphicsEngine.h"
//#include "InputManager.h"
//#include "Orxonox.h"
//#include "SpaceshipSteering.h"

#endif

#endif /* _OrxonoxStableHeaders_H__ */
