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

#include "OrxonoxStableHeaders.h"
#include "HUDSpeedBar.h"
#include "core/CoreIncludes.h"
#include "objects/worldentities/pawns/SpaceShip.h"
#include "objects/items/Engine.h"

namespace orxonox
{
    CreateFactory(HUDSpeedBar);

    HUDSpeedBar::HUDSpeedBar(BaseObject* creator)
        : HUDBar(creator)
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

        if (this->owner_ && this->owner_->getEngine())
        {
            float v = this->owner_->getVelocity().length();
            float value = v / (this->owner_->getEngine()->getMaxSpeedFront() * this->owner_->getEngine()->getSpeedFactor() * this->owner_->getEngine()->getBoostFactor());
            if (value != this->getValue())
                this->setValue(value);
        }
    }

    void HUDSpeedBar::changedOwner()
    {
        SUPER(HUDSpeedBar, changedOwner);

        this->owner_ = dynamic_cast<SpaceShip*>(this->getOwner());
    }
}
