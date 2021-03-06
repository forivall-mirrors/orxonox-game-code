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
 *      Matthias Spalinger
 *   Co-authors:
 *      Fabian 'x3n' Landau
 *
 */

#include "HUDEnemyHealthBar.h"

#include "core/config/ConfigValueIncludes.h"
#include "worldentities/pawns/Pawn.h"

namespace orxonox
{
    RegisterClass(HUDEnemyHealthBar);

    HUDEnemyHealthBar::HUDEnemyHealthBar(Context* context) : HUDHealthBar(context)
    {
        RegisterObject(HUDEnemyHealthBar);

        this->setConfigValues();
        this->owner_ = 0;
    }

    HUDEnemyHealthBar::~HUDEnemyHealthBar()
    {
    }

    void HUDEnemyHealthBar::setConfigValues()
    {
        SetConfigValue(useEnemyBar_, true);
    }

    void HUDEnemyHealthBar::tick(float dt)
    {
        this->updateTarget();

        SUPER(HUDEnemyHealthBar, tick, dt);
    }

    void HUDEnemyHealthBar::updateTarget()
    {
        Pawn* pawn = NULL;
        if (this->owner_ && this->useEnemyBar_)
        {
            // Get the owner's current target (target is usually a Model)
            WorldEntity* target = this->owner_->getTarget();
            // Find the Pawn that belongs to this target (if any)
            while (target && !target->isA(Class(Pawn)))
                target = target->getParent();
            pawn = orxonox_cast<Pawn*>(target);
            // Don't show the HealthBar if the pawn is invisible
            if (pawn && !pawn->isVisible())
                pawn = NULL;
        }
        // Set the pawn as owner of the HealthBar
        this->setHealthBarOwner(pawn);
        this->setVisible(pawn != NULL);
    }

    void HUDEnemyHealthBar::changedOwner()
    {
        SUPER(HUDEnemyHealthBar, changedOwner);

        this->owner_ = orxonox_cast<ControllableEntity*>(this->getOwner());
        this->updateTarget();
    }
}
