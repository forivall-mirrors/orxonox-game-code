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
    Shared library macros, enums, constants and forward declarations for the weapons module
*/

#ifndef _WeaponsPrereqs_H__
#define _WeaponsPrereqs_H__

#include "OrxonoxConfig.h"
#include "OrxonoxPrereqs.h"

//-----------------------------------------------------------------------
// Shared library settings
//-----------------------------------------------------------------------

#if defined(ORXONOX_PLATFORM_WINDOWS) && !defined(WEAPONS_STATIC_BUILD)
#  ifdef WEAPONS_SHARED_BUILD
#    define _WeaponsExport __declspec(dllexport)
#  else
#    if defined( __MINGW32__ )
#      define _WeaponsExport
#    else
#      define _WeaponsExport __declspec(dllimport)
#    endif
#  endif
#  define _WeaponsPrivate
#elif defined (ORXONOX_GCC_VISIBILITY)
#  define _WeaponsExport  __attribute__ ((visibility("default")))
#  define _WeaponsPrivate __attribute__ ((visibility("hidden")))
#else
#  define _WeaponsExport
#  define _WeaponsPrivate
#endif

//-----------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------

namespace orxonox
{
    class MuzzleFlash;

    // munitions
    class FusionMunition;
    class LaserMunition;
    class ReplenishingMunition;
    class RocketMunition;

    // projectiles
    class BillboardProjectile;
    class LightningGunProjectile;
    class ParticleProjectile;
    class Projectile;
    class Rocket;
    class SimpleRocket;

    // weaponmodes
    class EnergyDrink;
    class FusionFire;
    class HsW01;
    class LaserFire;
    class LightningGun;
    class RocketFire;
    class SimpleRocketFire;
}

#endif /* _WeaponsPrereqs_H__ */
