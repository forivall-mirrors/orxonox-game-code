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
#define ORXONOX_NO_EXPORTS // This is an executable that needs no exports
#if defined(ORXONOX_PLATFORM_WINDOWS) && !(defined(ORXONOX_STATIC_BUILD) || defined(ORXONOX_NO_EXPORTS))
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
    namespace LODParticle
    {
        enum LOD
        {
            off = 0,
            low = 1,
            normal = 2,
            high = 3
        };
    }

    class GraphicsManager;
    class OgreWindowEventListener;
    class Settings;

    class RadarViewable;
    class Radar;
    class RadarListener;

    class Teamcolourable;

    class CameraManager;
    class LevelManager;
    class PawnManager;
    class PlayerManager;

    // objects
    class Level;
    class Scene;

    class AddQuest;
    class AddQuestHint;
    class AddReward;
    class ChangeQuestStatus;
    class CompleteQuest;
    class FailQuest;
    class GlobalQuest;
    class LocalQuest;
    class Quest;
    class QuestDescription;
    class QuestEffect;
    class QuestEffectBeacon;
    class QuestHint;
    class QuestItem;
    class QuestListener;
    class QuestManager;
    class QuestNotification;
    class Rewardable;

    class WorldEntity;
    class StaticEntity;
    class MobileEntity;
    class ControllableEntity;
    class MovableEntity;
    class Sublevel;
    class ForceField;
    class Attacher;

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

    class PongCenterpoint;
    class PongBall;
    class PongBat;

    class Camera;
    class CameraPosition;
    class SpawnPoint;
    class TeamSpawnPoint;
    class Test;

    class Spectator;
    class Pawn;
    class SpaceShip;
    class TeamBaseMatchBase;
    class Destroyer;

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

    class Item;
    class Engine;
    class MultiStateEngine;
    class RotatingEngine;

    class Trigger;
    class DistanceTrigger;
    class EventTrigger;
    class PlayerTrigger;
    class CheckPoint;

    class WeaponSystem;
    class WeaponSet;
    class WeaponSlot;
    class WeaponPack;
    class Weapon;
    class WeaponMode;
    class DefaultWeaponmodeLink;
    class MuzzleFlash;

    class LaserFire;
    class FusionFire;
    class HsW01;
    class LightningGun;
    class EnergyDrink;

    class ReplenishingMunition;
    class Munition;
    class LaserMunition;
    class FusionMunition;

    class Projectile;
    class BillboardProjectile;
    class ParticleProjectile;
    class LightningGunProjectile;

    class EventListener;
    class EventDispatcher;
    class EventTarget;

    class Controller;
    class HumanController;
    class ArtificialController;
    class AIController;
    class ScriptController;
    class WaypointController;
    class WaypointPatrolController;
    class PongAI;

    class Info;
    class PlayerInfo;
    class HumanPlayer;
    class Bot;
    class PongBot;
    class GametypeInfo;

    class Gametype;
    class Deathmatch;
    class TeamDeathmatch;
    class Asteroids;
    class TeamBaseMatch;
    class UnderAttack;
    class Pong;

    class Scores;
    class CreateLines;
    class Scoreboard;
    class Stats;

    // collision
    class CollisionShape;
    class SphereCollisionShape;
    class CompoundCollisionShape;
    class PlaneCollisionShape;
    class WorldEntityCollisionShape;

    // tools
    class BillboardSet;
    class Light;
    class Mesh;
    class ParticleInterface;
    class Shader;
    template <class T>
    class Timer;
    class DynamicLines;
    class DynamicRenderable;

    // overlays
    class BarColour;
    class DebugFPSText;
    class DebugRTRText;
    class HUDBar;
    class HUDNavigation;
    class HUDRadar;
    class HUDSpeedBar;
    class HUDHealthBar;
    class HUDTimer;
    class InGameConsole;
    class Notification;
    class NotificationManager;
    class NotificationOverlay;
    class NotificationQueue;
    class OrxonoxOverlay;
    class OverlayGroup;
    class OverlayText;
    class GametypeStatus;
    class CreateLines;
    class Scoreboard;
    class Map;

    //gui
    class GUIManager;

    //sound
    class SoundBase;
    class SoundManager;
    class SoundMainMenu;
}

namespace Ogre
{
    // some got forgotten in OgrePrerequisites
    class BorderPanelOverlayElement;
    class PanelOverlayElement;
    class TextAreaOverlayElement;
}

namespace CEGUI
{
    class LuaScriptModule;

    class OgreCEGUIRenderer;
    class OgreCEGUIResourceProvider;
    class OgreCEGUITexture;
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

// lua
struct lua_State;

#endif /* _OrxonoxPrereqs_H__ */
