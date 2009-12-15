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

#include "HumanController.h"

#include "core/CoreIncludes.h"
#include "core/ConsoleCommand.h"
#include "worldentities/ControllableEntity.h"
#include "worldentities/pawns/Pawn.h"
#include "gametypes/Gametype.h"
#include "infos/PlayerInfo.h"
#include "overlays/Map.h"
#include "Radar.h"
#include "Scene.h"

namespace orxonox
{
    SetConsoleCommand(HumanController, moveFrontBack, true).setAsInputCommand();
    SetConsoleCommand(HumanController, moveRightLeft, true).setAsInputCommand();
    SetConsoleCommand(HumanController, moveUpDown,    true).setAsInputCommand();
    SetConsoleCommand(HumanController, rotateYaw,     true).setAsInputCommand();
    SetConsoleCommand(HumanController, rotatePitch,   true).setAsInputCommand();
    SetConsoleCommand(HumanController, rotateRoll,    true).setAsInputCommand();
    SetConsoleCommand(HumanController, fire,          true).keybindMode(KeybindMode::OnHold);
    SetConsoleCommand(HumanController, reload,        true);
    SetConsoleCommand(HumanController, boost,         true).keybindMode(KeybindMode::OnHold);
    SetConsoleCommand(HumanController, greet,         true);
    SetConsoleCommand(HumanController, switchCamera,  true);
    SetConsoleCommand(HumanController, mouseLook,     true);
    SetConsoleCommand(HumanController, suicide,       true);
    SetConsoleCommand(HumanController, toggleGodMode, true);
    SetConsoleCommand(HumanController, addBots,       true).defaultValues(1);
    SetConsoleCommand(HumanController, killBots,      true).defaultValues(0);
    SetConsoleCommand(HumanController, dropItems,     true);
    SetConsoleCommand(HumanController, useItem,       true);
    SetConsoleCommand(HumanController, cycleNavigationFocus,   true);
    SetConsoleCommand(HumanController, releaseNavigationFocus, true);

    CreateUnloadableFactory(HumanController);

    HumanController* HumanController::localController_s = 0;

    HumanController::HumanController(BaseObject* creator) : Controller(creator)
    {
        RegisterObject(HumanController);

        controlPaused_ = false;

        HumanController::localController_s = this;
    }

    HumanController::~HumanController()
    {
        HumanController::localController_s = 0;
    }

    void HumanController::tick(float dt)
    {
        if (GameMode::playsSound() && HumanController::localController_s && HumanController::localController_s->controllableEntity_)
        {
            Camera* camera = HumanController::localController_s->controllableEntity_->getCamera();
            if (!camera)
                COUT(3) << "HumanController, Warning: Using a ControllableEntity without Camera" << std::endl;
        }
    }

    void HumanController::moveFrontBack(const Vector2& value)
    {
        if (HumanController::localController_s)
            HumanController::localController_s->frontback(value);
    }

    void HumanController::frontback(const Vector2& value)
    {
        if (HumanController::localController_s && HumanController::localController_s->controllableEntity_)
            HumanController::localController_s->controllableEntity_->moveFrontBack(value);
    }

    void HumanController::moveRightLeft(const Vector2& value)
    {
        if (HumanController::localController_s && HumanController::localController_s->controllableEntity_)
            HumanController::localController_s->controllableEntity_->moveRightLeft(value);
    }

    void HumanController::moveUpDown(const Vector2& value)
    {
        if (HumanController::localController_s && HumanController::localController_s->controllableEntity_)
            HumanController::localController_s->controllableEntity_->moveUpDown(value);
    }

    void HumanController::yaw(const Vector2& value)
    {
        //Hack to enable mouselook in map
        if ( Map::getSingletonPtr() && Map::getSingletonPtr()->getVisibility() && HumanController::localController_s->controllableEntity_->isInMouseLook() )
        {
            Map::getSingletonPtr()->rotateYaw(value);
            return;
        }
        if (HumanController::localController_s && HumanController::localController_s->controllableEntity_)
            HumanController::localController_s->controllableEntity_->rotateYaw(value);
    }

    void HumanController::pitch(const Vector2& value)
    {
        //Hack to enable mouselook in map
        if ( Map::getSingletonPtr() && Map::getSingletonPtr()->getVisibility() && HumanController::localController_s->controllableEntity_->isInMouseLook() )
        {
            Map::getSingletonPtr()->rotatePitch(value);
            return;
        }
        if (HumanController::localController_s && HumanController::localController_s->controllableEntity_)
            HumanController::localController_s->controllableEntity_->rotatePitch(value);
    }

    void HumanController::rotateYaw(const Vector2& value)
    {
        if (HumanController::localController_s)
            HumanController::localController_s->yaw(value);
    }

    void HumanController::rotatePitch(const Vector2& value)
    {
        if (HumanController::localController_s)
            HumanController::localController_s->pitch(value);
    }

    void HumanController::rotateRoll(const Vector2& value)
    {
        if (HumanController::localController_s && HumanController::localController_s->controllableEntity_)
            HumanController::localController_s->controllableEntity_->rotateRoll(value);
    }

    void HumanController::fire(unsigned int firemode)
    {
        if (HumanController::localController_s)
            HumanController::localController_s->doFire(firemode);
    }

    void HumanController::doFire(unsigned int firemode)
    {
        if (HumanController::localController_s && HumanController::localController_s->controllableEntity_)
            HumanController::localController_s->controllableEntity_->fire(firemode);
    }

    void HumanController::reload()
    {
        if (HumanController::localController_s && HumanController::localController_s->controllableEntity_)
            HumanController::localController_s->controllableEntity_->reload();
    }

    void HumanController::boost()
    {
        if (HumanController::localController_s && HumanController::localController_s->controllableEntity_)
            HumanController::localController_s->controllableEntity_->boost();
    }

    void HumanController::greet()
    {
        if (HumanController::localController_s && HumanController::localController_s->controllableEntity_)
            HumanController::localController_s->controllableEntity_->greet();
    }

    void HumanController::switchCamera()
    {
        if (HumanController::localController_s && HumanController::localController_s->controllableEntity_)
            HumanController::localController_s->controllableEntity_->switchCamera();
    }

    void HumanController::mouseLook()
    {
        if (HumanController::localController_s && HumanController::localController_s->controllableEntity_)
            HumanController::localController_s->controllableEntity_->mouseLook();
    }

    void HumanController::suicide()
    {
        if (HumanController::localController_s && HumanController::localController_s->controllableEntity_)
        {
            Pawn* pawn = orxonox_cast<Pawn*>(HumanController::localController_s->controllableEntity_);
            if (pawn)
                pawn->kill();
            else if (HumanController::localController_s->player_)
                HumanController::localController_s->player_->stopControl();
        }
    }

    void HumanController::toggleGodMode()
    {
        HumanController::getLocalControllerSingleton()->setGodMode( !HumanController::getLocalControllerSingleton()->getGodMode() );
    }

    void HumanController::useItem()
    {
        if (HumanController::localController_s && HumanController::localController_s->controllableEntity_)
            HumanController::localController_s->controllableEntity_->useItem();
    }

    void HumanController::addBots(unsigned int amount)
    {
        if (HumanController::localController_s && HumanController::localController_s->controllableEntity_ && HumanController::localController_s->controllableEntity_->getGametype())
            HumanController::localController_s->controllableEntity_->getGametype()->addBots(amount);
    }

    void HumanController::killBots(unsigned int amount)
    {
        if (HumanController::localController_s && HumanController::localController_s->controllableEntity_ && HumanController::localController_s->controllableEntity_->getGametype())
            HumanController::localController_s->controllableEntity_->getGametype()->killBots(amount);
    }

    void HumanController::dropItems()
    {
        if (HumanController::localController_s && HumanController::localController_s->controllableEntity_)
            HumanController::localController_s->controllableEntity_->dropItems();
    }

    Pawn* HumanController::getLocalControllerEntityAsPawn()
    {
        if (HumanController::localController_s)
            return orxonox_cast<Pawn*>(HumanController::localController_s->getControllableEntity());
        else
            return NULL;
    }

    void HumanController::cycleNavigationFocus()
    {
        if (HumanController::localController_s && HumanController::localController_s->controllableEntity_)
            HumanController::localController_s->controllableEntity_->getScene()->getRadar()->cycleFocus();
    }

    void HumanController::releaseNavigationFocus()
    {
        if (HumanController::localController_s && HumanController::localController_s->controllableEntity_)
            HumanController::localController_s->controllableEntity_->getScene()->getRadar()->releaseFocus();
    }

    void HumanController::pauseControl()
    {
        if (HumanController::localController_s)
            HumanController::localController_s->doPauseControl();
    }

    void HumanController::resumeControl()
    {
        if (HumanController::localController_s)
            HumanController::localController_s->doResumeControl();
    }
}
