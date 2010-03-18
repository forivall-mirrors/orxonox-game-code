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

#include "GametypeStatus.h"

#include "util/Convert.h"
#include "core/CoreIncludes.h"
#include "infos/GametypeInfo.h"
#include "infos/PlayerInfo.h"
#include "worldentities/ControllableEntity.h"
#include "worldentities/pawns/Spectator.h"

namespace orxonox
{
    CreateFactory(GametypeStatus);

    GametypeStatus::GametypeStatus(BaseObject* creator) : OverlayText(creator)
    {
        RegisterObject(GametypeStatus);

        this->owner_ = 0;
    }

    GametypeStatus::~GametypeStatus()
    {
    }

    void GametypeStatus::tick(float dt)
    {
        SUPER(GametypeStatus, tick, dt);

        if (this->owner_ && this->owner_->getGametypeInfo() && this->owner_->getControllableEntity())
        {
            const GametypeInfo* gtinfo = this->owner_->getGametypeInfo();
            ControllableEntity* ce = this->owner_->getControllableEntity();

            if (!gtinfo->hasStarted() && !gtinfo->isStartCountdownRunning())
            {
                if (!this->owner_->isReadyToSpawn())
                    this->setCaption("Press [Fire] to start the match");
                else
                    this->setCaption("Waiting for other players");
            }
            else if (!gtinfo->hasEnded())
            {
                if (gtinfo->isStartCountdownRunning())
                    this->setCaption(multi_cast<std::string>(static_cast<int>(ceil(gtinfo->getStartCountdown()))));
                else if (ce->isA(Class(Spectator)))
                    this->setCaption("Press [Fire] to respawn");
                else
                    this->setCaption("");
            }
            else
                this->setCaption("Game has ended");
        }
    }


    void GametypeStatus::changedOwner()
    {
        SUPER(GametypeStatus, changedOwner);

        this->owner_ = orxonox_cast<PlayerInfo*>(this->getOwner());
    }
}
