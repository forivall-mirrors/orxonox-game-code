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
#include "core/command/ConsoleCommand.h"
#include "worldentities/ControllableEntity.h"
#include "worldentities/pawns/Pawn.h"
#include "gametypes/Gametype.h"
#include "infos/PlayerInfo.h"
#include "Radar.h"
#include "Scene.h"

namespace orxonox
{
    extern const std::string __CC_fire_name = "fire";
    extern const std::string __CC_suicide_name = "suicide";
    const std::string __CC_boost_name = "boost";

    SetConsoleCommand("HumanController", "moveFrontBack",          &HumanController::moveFrontBack ).addShortcut().setAsInputCommand();
    SetConsoleCommand("HumanController", "moveRightLeft",          &HumanController::moveRightLeft ).addShortcut().setAsInputCommand();
    SetConsoleCommand("HumanController", "moveUpDown",             &HumanController::moveUpDown    ).addShortcut().setAsInputCommand();
    SetConsoleCommand("HumanController", "rotateYaw",              &HumanController::rotateYaw     ).addShortcut().setAsInputCommand();
    SetConsoleCommand("HumanController", "rotatePitch",            &HumanController::rotatePitch   ).addShortcut().setAsInputCommand();
    SetConsoleCommand("HumanController", "rotateRoll",             &HumanController::rotateRoll    ).addShortcut().setAsInputCommand();
    SetConsoleCommand("HumanController", "toggleFormationFlight",  &HumanController::toggleFormationFlight).addShortcut().setAsInputCommand();
    SetConsoleCommand("HumanController", __CC_fire_name,           &HumanController::fire          ).addShortcut().keybindMode(KeybindMode::OnHold);
    SetConsoleCommand("HumanController", "reload",                 &HumanController::reload        ).addShortcut();
    SetConsoleCommand("HumanController", __CC_boost_name,          &HumanController::keepBoost     ).addShortcut().keybindMode(KeybindMode::OnHold);
    SetConsoleCommand("HumanController", "greet",                  &HumanController::greet         ).addShortcut();
    SetConsoleCommand("HumanController", "switchCamera",           &HumanController::switchCamera  ).addShortcut();
    SetConsoleCommand("HumanController", "mouseLook",              &HumanController::mouseLook     ).addShortcut();
    SetConsoleCommand("HumanController", __CC_suicide_name,        &HumanController::suicide       ).addShortcut();
    SetConsoleCommand("HumanController", "toggleGodMode",          &HumanController::toggleGodMode ).addShortcut();
    SetConsoleCommand("HumanController", "addBots",                &HumanController::addBots       ).addShortcut().defaultValues(1);
    SetConsoleCommand("HumanController", "killBots",               &HumanController::killBots      ).addShortcut().defaultValues(0);
    SetConsoleCommand("HumanController", "cycleNavigationFocus",   &HumanController::cycleNavigationFocus).addShortcut();
    SetConsoleCommand("HumanController", "releaseNavigationFocus", &HumanController::releaseNavigationFocus).addShortcut();
    SetConsoleCommand("HumanController", "myposition",             &HumanController::myposition    ).addShortcut();

    CreateUnloadableFactory(HumanController);

    HumanController* HumanController::localController_s = 0;
    /*static*/ const float HumanController::BOOSTING_TIME = 0.1f;

    HumanController::HumanController(BaseObject* creator) : Masterable(creator)
    {
        RegisterObject(HumanController);

        this->controlPaused_ = false;
        this->boosting_ = false;
        this->boosting_ = false;

        HumanController::localController_s = this;
        this->boostingTimeout_.setTimer(HumanController::BOOSTING_TIME, false, createExecutor(createFunctor(&HumanController::terminateBoosting, this)));
        this->boostingTimeout_.stopTimer();
        this->state_=MASTER;
    }

    HumanController::~HumanController()
    {
        HumanController::localController_s = 0;
        if (this->state_==MASTER)
            freeSlaves();
    }

    void HumanController::tick(float dt)
    {
        if (GameMode::playsSound() && HumanController::localController_s && HumanController::localController_s->controllableEntity_)
        {
            Camera* camera = HumanController::localController_s->controllableEntity_->getCamera();
            if (!camera)
                orxout(internal_warning) << "HumanController, Warning: Using a ControllableEntity without Camera" << endl;
        }

        // commandslaves when Master of a formation
        if (this->state_==MASTER)
        {
            this->commandSlaves();
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
        if (HumanController::localController_s && HumanController::localController_s->controllableEntity_)
            HumanController::localController_s->controllableEntity_->rotateYaw(value);
    }

    void HumanController::pitch(const Vector2& value)
    {
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

    /**
    @brief
        Static method,keeps boosting.
    */
    /*static*/ void HumanController::keepBoost()
    {
        if (HumanController::localController_s && HumanController::localController_s->controllableEntity_)
            HumanController::localController_s->keepBoosting();
    }
    
    /**
    @brief
        Starts, or keeps the boosting mode.
        Resets the boosting timeout and ells the ControllableEntity to boost (or not boost anymore).
    */
    void HumanController::keepBoosting(void)
    {
        if(this->boostingTimeout_.isActive())
        {
            this->boostingTimeout_.stopTimer();
            this->boostingTimeout_.startTimer();
        }
        else
        {
            this->boosting_ = true;
            this->boostingTimeout_.startTimer();
            
            this->controllableEntity_->boost(this->boosting_);
//            orxout() << "Start boosting" << endl;
        }
    }

    /**
    @brief
        Terminates the boosting mode.
    */
    void HumanController::terminateBoosting(void)
    {
        this->boosting_ = false;
        this->boostingTimeout_.stopTimer();

        this->controllableEntity_->boost(this->boosting_);
//        orxout() << "Stop boosting" << endl;
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
        if (HumanController::localController_s)
            HumanController::localController_s->setGodMode(!HumanController::localController_s->getGodMode());
    }

    void HumanController::myposition()
    {
        if (HumanController::localController_s && HumanController::localController_s->controllableEntity_)
        {
            const Vector3& position = HumanController::localController_s->controllableEntity_->getPosition();
            const Quaternion& orientation = HumanController::localController_s->controllableEntity_->getOrientation();

            orxout(message) << "position=\"" << position.x << ", " << position.y << ", " << position.z << "\" "
                            << "orientation=\"" << orientation.w << ", " << orientation.x << ", " << orientation.y << ", " << orientation.z << "\"" << endl;
        }
    }

    void HumanController::toggleFormationFlight()
    {
        if (HumanController::localController_s)
        {
            if (HumanController::localController_s->state_==MASTER)
            {
                HumanController::localController_s->freeSlaves();
                HumanController::localController_s->state_=FREE;
            } else //SLAVE or FREE
            {
                HumanController::localController_s->takeLeadOfFormation();
                //HumanController::localController_s->state_=MASTER;
            }
        }

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
