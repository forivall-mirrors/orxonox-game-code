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
    Shared library macros, enums, constants and forward declarations for the orxonox library
*/

#ifndef _OrxonoxPrereqs_H__
#define _OrxonoxPrereqs_H__

#include "OrxonoxConfig.h"
#include "tools/ToolsPrereqs.h"

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
#  define _OrxonoxPrivate
#elif defined (ORXONOX_GCC_VISIBILITY)
#  define _OrxonoxExport  __attribute__ ((visibility("default")))
#  define _OrxonoxPrivate __attribute__ ((visibility("hidden")))
#else
#  define _OrxonoxExport
#  define _OrxonoxPrivate
#endif

//-----------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------

namespace orxonox
{
    class CameraManager;
    class Level;
    class LevelInfo;
    class LevelInfoItem;
    class LevelManager;
    class PawnManager;
    class PlayerManager;
    class Radar;
    class RenderQueueListener;
    class Scene;

    // chat
    class ChatHistory;
    class ChatInputHandler;
    class ChatListener;
    class ChatManager;

    // collisionshapes
    class CollisionShape;
    class CompoundCollisionShape;
    class WorldEntityCollisionShape;

    // controllers
    class AIController;
    class ArtificialController;
    class Controller;
    class DroneController;
    class HumanController;
    class ScriptController;
    class WaypointController;
    class WaypointPatrolController;

    // gametypes
    class Asteroids;
    class Deathmatch;
    class Dynamicmatch;
    class Gametype;
    class LastManStanding;
    class LastTeamStanding;
    class TeamBaseMatch;
    class TeamDeathmatch;
    class UnderAttack;
    class TeamGametype;

    // graphics
    class Backlight;
    class Billboard;
    class BlinkingBillboard;
    class Camera;
    class FadingBillboard;
    class GlobalShader;
    class Light;
    class Model;
    class ParticleEmitter;
    class ParticleSpawner;

    // infos
    class Bot;
    class GametypeInfo;
    class HumanPlayer;
    class Info;
    class PlayerInfo;

    // interfaces
    class GametypeMessageListener;
    class NotificationListener;
    class Pickupable;
    class PickupCarrier;
    class PlayerTrigger;
    class RadarListener;
    class RadarViewable;
    class Rewardable;
    class TeamColourable;

    // items
    class Engine;
    class Item;
    class MultiStateEngine;

    // overlays
    class InGameConsole;
    class Map;
    class OrxonoxOverlay;
    class OverlayGroup;

    //sound
    class AmbientSound;
    class BaseSound;
    class SoundBuffer;
    class SoundManager;
    class SoundStreamer;
    class WorldSound;
    class WorldAmbientSound;

    // weaponsystem
    class DefaultWeaponmodeLink;
    class Munition;
    class Weapon;
    class WeaponMode;
    class WeaponPack;
    class WeaponSet;
    class WeaponSlot;
    class WeaponSystem;

    // worldentities
    class BigExplosion;
    class CameraPosition;
    class ControllableEntity;
    class Drone;
    class EffectContainer;
    class ExplosionChunk;
    class MobileEntity;
    class MovableEntity;
    class SpawnPoint;
    class StaticEntity;
    class TeamSpawnPoint;
    class WorldEntity;
    class Rocket;
    // worldentities, pawns
    class Destroyer;
    class Pawn;
    class SpaceShip;
    class Spectator;
    class TeamBaseMatchBase;
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
typedef int ALenum;

#endif /* _OrxonoxPrereqs_H__ */
