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
 *      Felix Schulthess
 *      Reto Grieder
 *   Co-authors:
 *      ...
 *
 */

#include "HUDSpeedBar.h"

#include "core/CoreIncludes.h"
#include "worldentities/pawns/SpaceShip.h"
#include "items/Engine.h"

namespace orxonox
{
    RegisterClass(HUDSpeedBar);

    HUDSpeedBar::HUDSpeedBar(Context* context)
        : HUDBar(context)
    {
        RegisterObject(HUDSpeedBar);

        this->owner_ = 0;
    }

    HUDSpeedBar::~HUDSpeedBar()
    {
    }

    void HUDSpeedBar::tick(float dt)
    {
        SUPER(HUDSpeedBar, tick, dt);

        if (this->owner_)
        {
            float value = this->owner_->getVelocity().length() / (this->owner_->getMaxSpeedFront() * this->owner_->getSpeedFactor() * this->owner_->getBoostFactor());
            this->setValue(value);
        }
    }

    void HUDSpeedBar::changedOwner()
    {
        SUPER(HUDSpeedBar, changedOwner);

        this->owner_ = orxonox_cast<SpaceShip*>(this->getOwner());
    }
}
