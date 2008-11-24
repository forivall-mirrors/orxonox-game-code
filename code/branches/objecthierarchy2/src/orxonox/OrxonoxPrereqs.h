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

    class GraphicsEngine;
    class Settings;

    class RadarViewable;
    class Radar;
    class RadarListener;

    class CameraManager;
    class LevelManager;
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
    class QuestHint;
    class QuestItem;
    class QuestManager;
    class Rewardable;

    class WorldEntity;
    class PositionableEntity;
    class MovableEntity;
    class ControllableEntity;
    class Sublevel;

    class Model;
    class Billboard;
    class BlinkingBillboard;
    class FadingBillboard;
    class Light;
    class Backlight;
    class ParticleEmitter;
    class ParticleSpawner;

    class Camera;
    class CameraPosition;
    class SpawnPoint;

    class Spectator;
    class Pawn;
    class SpaceShip;

    class Item;
    class Engine;
    class MultiStateEngine;
    class RotatingEngine;

    class Trigger;
    class DistanceTrigger;
    class EventTrigger;

    class WeaponSystem;
    class WeaponSet;
    class WeaponSlot;
    class Weapon;
    class Munition;

    class EventListener;
    class EventDispatcher;
    class EventTarget;

    class Controller;
    class HumanController;

    class Info;
    class PlayerInfo;
    class HumanPlayer;

    class Gametype;

    class Scores;

    // tools
    class BillboardSet;
    class Light;
    class Mesh;
    class ParticleInterface;
    template <class T>
    class Timer;

    // overlays
    class BarColour;
    class DebugFPSText;
    class DebugRTRText;
    class HUDBar;
    class HUDNavigation;
    class HUDRadar;
    class HUDSpeedBar;
    class InGameConsole;
    class OrxonoxOverlay;
    class OverlayGroup;
    class OverlayText;

    //gui
    class GUIManager;

    // game states
    class GSRoot;
    class GSGraphics;
    class GSIO;
    class GSIOConsole;
    class GSLevel;
    class GSStandalone;
    class GSServer;
    class GSClient;
    class GSGUI;
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

struct lua_State;

#endif /* _OrxonoxPrereqs_H__ */
