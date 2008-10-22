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

#include "OrxonoxStableHeaders.h"
#include "HumanController.h"

#include "core/CoreIncludes.h"
#include "core/ConsoleCommand.h"
#include "objects/worldentities/ControllableEntity.h"

namespace orxonox
{
    SetConsoleCommand(HumanController, moveFrontBack, true).setAsInputCommand();
    SetConsoleCommand(HumanController, moveRightLeft, true).setAsInputCommand();
    SetConsoleCommand(HumanController, moveUpDown,    true).setAsInputCommand();
    SetConsoleCommand(HumanController, rotateYaw,     true).setAsInputCommand();
    SetConsoleCommand(HumanController, rotatePitch,   true).setAsInputCommand();
    SetConsoleCommand(HumanController, rotateRoll,    true).setAsInputCommand();
    SetConsoleCommand(HumanController, fire,          true).keybindMode(KeybindMode::OnHold);
    SetConsoleCommand(HumanController, altFire,       true).keybindMode(KeybindMode::OnHold);
    SetConsoleCommand(HumanController, greet,         true);
    SetConsoleCommand(HumanController, use,           true);
    SetConsoleCommand(HumanController, switchCamera,  true);

    CreateUnloadableFactory(HumanController);

    HumanController* HumanController::localController_s = 0;

    HumanController::HumanController()
    {
        RegisterObject(HumanController);

        HumanController::localController_s = this;
    }

    HumanController::~HumanController()
    {
        HumanController::localController_s = 0;
    }

    void HumanController::moveFrontBack(const Vector2& value)
    {
        if (HumanController::localController_s && HumanController::localController_s->pawn_)
            HumanController::localController_s->pawn_->moveFrontBack(value.y);
    }

    void HumanController::moveRightLeft(const Vector2& value)
    {
        if (HumanController::localController_s && HumanController::localController_s->pawn_)
            HumanController::localController_s->pawn_->moveRightLeft(value.y);
    }

    void HumanController::moveUpDown(const Vector2& value)
    {
        if (HumanController::localController_s && HumanController::localController_s->pawn_)
            HumanController::localController_s->pawn_->moveUpDown(value.y);
    }

    void HumanController::rotateYaw(const Vector2& value)
    {
        if (HumanController::localController_s && HumanController::localController_s->pawn_)
            HumanController::localController_s->pawn_->rotateYaw(value.y);
    }

    void HumanController::rotatePitch(const Vector2& value)
    {
        if (HumanController::localController_s && HumanController::localController_s->pawn_)
            HumanController::localController_s->pawn_->rotatePitch(value.y);
    }

    void HumanController::rotateRoll(const Vector2& value)
    {
        if (HumanController::localController_s && HumanController::localController_s->pawn_)
            HumanController::localController_s->pawn_->rotateRoll(value.y);
    }

    void HumanController::fire()
    {
        if (HumanController::localController_s && HumanController::localController_s->pawn_)
            HumanController::localController_s->pawn_->fire();
    }

    void HumanController::altFire()
    {
        if (HumanController::localController_s && HumanController::localController_s->pawn_)
            HumanController::localController_s->pawn_->altFire();
    }

    void HumanController::greet()
    {
        if (HumanController::localController_s && HumanController::localController_s->pawn_)
            HumanController::localController_s->pawn_->greet();
    }

    void HumanController::use()
    {
        if (HumanController::localController_s && HumanController::localController_s->pawn_)
            HumanController::localController_s->pawn_->use();
    }

    void HumanController::switchCamera()
    {
        if (HumanController::localController_s && HumanController::localController_s->pawn_)
            HumanController::localController_s->pawn_->switchCamera();
    }
}
