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
 *      Fabian 'x3n' Landau
 *   Co-authors:
 *      ...
 *
 */

#include "NewHumanController.h"

#include "core/CoreIncludes.h"
#include "core/ConsoleCommand.h"
#include "worldentities/ControllableEntity.h"
#include "worldentities/pawns/Pawn.h"
#include "gametypes/Gametype.h"
#include "infos/PlayerInfo.h"
#include "overlays/Map.h"
#include "graphics/Camera.h"
#include "sound/SoundManager.h"
#include "Radar.h"
#include "Scene.h"

namespace orxonox
{
    SetConsoleCommand(NewHumanController, moveFrontBack, true).setAsInputCommand();
    SetConsoleCommand(NewHumanController, moveRightLeft, true).setAsInputCommand();
    SetConsoleCommand(NewHumanController, moveUpDown,    true).setAsInputCommand();
    SetConsoleCommand(NewHumanController, rotateYaw,     true).setAsInputCommand();
    SetConsoleCommand(NewHumanController, rotatePitch,   true).setAsInputCommand();
    SetConsoleCommand(NewHumanController, rotateRoll,    true).setAsInputCommand();
    SetConsoleCommand(NewHumanController, fire,          true).keybindMode(KeybindMode::OnHold);
    SetConsoleCommand(NewHumanController, reload,        true);
    SetConsoleCommand(NewHumanController, boost,         true).keybindMode(KeybindMode::OnHold);
    SetConsoleCommand(NewHumanController, greet,         true);
    SetConsoleCommand(NewHumanController, switchCamera,  true);
    SetConsoleCommand(NewHumanController, mouseLook,     true);
    SetConsoleCommand(NewHumanController, suicide,       true);
    SetConsoleCommand(NewHumanController, addBots,       true).defaultValues(1);
    SetConsoleCommand(NewHumanController, killBots,      true).defaultValues(0);
    SetConsoleCommand(NewHumanController, dropItems,     true);
    SetConsoleCommand(NewHumanController, useItem,       true);
    SetConsoleCommand(NewHumanController, cycleNavigationFocus,   true);
    SetConsoleCommand(NewHumanController, releaseNavigationFocus, true);

    CreateUnloadableFactory(NewHumanController);

    NewHumanController* NewHumanController::localController_s = 0;

    NewHumanController::NewHumanController(BaseObject* creator) : HumanController(creator)
    {
        RegisterObject(NewHumanController);

        NewHumanController::localController_s = this;
    }

    NewHumanController::~NewHumanController()
    {
        NewHumanController::localController_s = 0;
    }

    void NewHumanController::tick(float dt)
    {
        if (GameMode::playsSound() && NewHumanController::localController_s && NewHumanController::localController_s->controllableEntity_)
        {
            // Update sound listener
            Camera* camera = NewHumanController::localController_s->controllableEntity_->getCamera();
            if (camera)
            {
                SoundManager::getInstance().setListenerPosition(camera->getWorldPosition());
                SoundManager::getInstance().setListenerOrientation(camera->getWorldOrientation());
            }
            else
                COUT(3) << "NewHumanController, Warning: Using a ControllableEntity without Camera" << std::endl;
        }
    }

}
