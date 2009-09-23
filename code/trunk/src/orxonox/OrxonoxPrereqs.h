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

#include "OrxonoxConfig.h"

//-----------------------------------------------------------------------
// Shared library settings
//-----------------------------------------------------------------------
#if defined(ORXONOX_PLATFORM_WINDOWS) && !defined(ORXONOX_STATIC_BUILD)
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

namespace orxonox
{
    class CameraManager;
    class LevelManager;
    class PawnManager;
    class PlayerManager;

    class Level;
    class Scene;
    class Tickable;

    // interfaces
    class GametypeMessageListener;
    class NotificationListener;
    class PawnListener;
    class PlayerTrigger;
    class RadarListener;
    class RadarViewable;
    class Rewardable;
    class Teamcolourable;

    // worldentities
    class WorldEntity;
    class StaticEntity;
    class MobileEntity;
    class ControllableEntity;
    class MovableEntity;

    // graphics
    class Model;
    class Billboard;
    class BlinkingBillboard;
    class BigExplosion;
    class ExplosionChunk;
    class FadingBillboard;
    class GlobalShader;
    class Light;
    class Backlight;
    class ParticleEmitter;
    class ParticleSpawner;
    class Camera;

    // mixed
    class SpawnPoint;
    class TeamSpawnPoint;

    class CameraPosition;
    class Sublevel;
    class Radar;

    class Test;

    // pawns
    class Spectator;
    class Pawn;
    class SpaceShip;
    class TeamBaseMatchBase;
    class Destroyer;

    // gametypes
    class Gametype;
    class Deathmatch;
    class TeamDeathmatch;
    class Asteroids;
    class TeamBaseMatch;
    class UnderAttack;

    // pickups
    class BaseItem;
    class DroppedItem;
    class EquipmentItem;
    class ModifierPickup;
    class PassiveItem;
    class PickupCollection;
    class PickupInventory;
    class PickupSpawner;
    class UsableItem;

    class Jump;
    class HealthUsable;
    class PassiveItem;

    // items
    class Item;
    class Engine;
    class MultiStateEngine;
    class RotatingEngine;

    // weaponsystem
    class WeaponSystem;
    class WeaponSet;
    class WeaponSlot;
    class WeaponPack;
    class Weapon;
    class WeaponMode;
    class DefaultWeaponmodeLink;
    class Munition;

    // controller
    class Controller;
    class HumanController;
    class ArtificialController;
    class AIController;
    class ScriptController;
    class WaypointController;
    class WaypointPatrolController;

    // infos
    class Info;
    class PlayerInfo;
    class HumanPlayer;
    class Bot;
    class GametypeInfo;

    // collision
    class CollisionShape;
    class CompoundCollisionShape;
    class WorldEntityCollisionShape;

    // overlays
    class OverlayGroup;
    class OrxonoxOverlay;
    class Notification;
    class NotificationManager;
    class InGameConsole;
    class Map;

    //sound
    class SoundBase;
    class SoundManager;
    class SoundMainMenu;
}

// Bullet Physics Engine
class btTransform;
class btVector3;

class btRigidBody;
class btCollisionObject;
class btGhostObject;
class btManifoldPoint;

class btCollisionShape;
class btSphereShape;
class btCompoundShape;
class btStaticPlaneShape;

class btDiscreteDynamicsWorld;
class bt32BitAxisSweep3;
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btSequentialImpulseConstraintSolver;

// ALUT
typedef struct ALCcontext_struct ALCcontext;
typedef struct ALCdevice_struct ALCdevice;
typedef unsigned int ALuint;
typedef int ALint;

#endif /* _OrxonoxPrereqs_H__ */
