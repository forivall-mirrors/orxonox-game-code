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
 *      Aurelian Jaggi
 *   Co-authors:
 *      ...
 *
 */

#include "HUDTimer.h"

#include "util/Convert.h"
#include "core/CoreIncludes.h"
#include "worldentities/ControllableEntity.h"
#include "gametypes/Gametype.h"

namespace orxonox
{
    RegisterClass(HUDTimer);

    HUDTimer::HUDTimer(Context* context) : OverlayText(context)
    {
        RegisterObject(HUDTimer);

        this->owner_ = 0;
    }

    HUDTimer::~HUDTimer()
    {
    }

    void HUDTimer::tick(float dt)
    {
        SUPER(HUDTimer, tick, dt);

        Gametype* gametype = this->getGametype();

        if (gametype)
        {
            if (gametype->getTimerIsActive())
            {
                this->setCaption(multi_cast<std::string>(static_cast<int>(gametype->getTime()) + 1));
            }
        }
    }

    void HUDTimer::changedOwner()
    {
        SUPER(HUDTimer, changedOwner);

        this->owner_ = orxonox_cast<ControllableEntity*>(this->getOwner());
    }
}
