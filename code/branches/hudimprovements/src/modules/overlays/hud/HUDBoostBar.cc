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
 *      Benjamin Knecht
 *   Co-authors:
 *      ...
 *
 */

#include "HUDBoostBar.h"

#include "core/CoreIncludes.h"
#include "worldentities/pawns/SpaceShip.h"
#include "items/Engine.h"

namespace orxonox
{
    CreateFactory(HUDBoostBar);

    HUDBoostBar::HUDBoostBar(BaseObject* creator)
        : HUDBar(creator)
    {
        RegisterObject(HUDBoostBar);

        this->owner_ = 0;
    }

    HUDBoostBar::~HUDBoostBar()
    {
    }

    void HUDBoostBar::tick(float dt)
    {
        SUPER(HUDBoostBar, tick, dt);

        if (this->owner_ /*&& !this->owner_->getPermanentBoost()*/)
        {
            this->show();
            float value = this->owner_->getBoostPower() / this->owner_->getInitialBoostPower();
            this->setValue(value);
        }
        /*else
            this->hide();*/
    }

    void HUDBoostBar::changedOwner()
    {
        SUPER(HUDBoostBar, changedOwner);

        this->owner_ = orxonox_cast<SpaceShip*>(this->getOwner());
    }
}

