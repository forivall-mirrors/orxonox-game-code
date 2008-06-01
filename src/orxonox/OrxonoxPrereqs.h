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
  @brief Contains all the necessary forward declarations for all classes and structs.
*/

#ifndef _OrxonoxPrereqs_H__
#define _OrxonoxPrereqs_H__

#include "util/OrxonoxPlatform.h"

//-----------------------------------------------------------------------
// Shared library settings
//-----------------------------------------------------------------------
#if (ORXONOX_PLATFORM == ORXONOX_PLATFORM_WIN32) && !(defined(ORXONOX_STATIC_BUILD) || defined(ORXONOX_NO_EXPORTS))
#  ifdef ORXONOX_SHARED_BUILD
#    define _OrxonoxExport __declspec(dllexport)
#  else
#    if defined( __MINGW32__ )
#      define _OrxonoxExport
#    else
#      define _OrxonoxExport __declspec(dllimport)
#    endif
#  endif
#elif defined ( ORXONOX_GCC_VISIBILITY )
#  define _OrxonoxExport  __attribute__ ((visibility("default")))
#else
#  define _OrxonoxExport
#endif

//-----------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------

namespace orxonox {
  class GraphicsEngine;
  class Orxonox;

  // objects
  class Ambient;
  class Camera;
  class Explosion;
  class Fighter;
  class Model;
  class NPC;
  class Projectile;
  class Skybox;
  class SpaceShip;
  class SpaceShipAI;
  class WorldEntity;

  class AmmunitionDump;
  class Bullet;
  class BulletManager;
  class BaseWeapon;
  class BarrelGun;
  class WeaponStation;

  // tools
  class BillboardSet;
  class Light;
  class Mesh;
  template <class T>
  class Timer;
  class TimerBase;

  // particle
  class ParticleInterface;

  // hud
  class BarOverlayElement;
  class HUD;
  class Navigation;
  class RadarObject;
  class RadarOverlayElement;

  //console
  class InGameConsole;
}


#endif /* _OrxonoxPrereqs_H__ */
