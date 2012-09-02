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
#include "core/command/ConsoleCommand.h"
#include "infos/GametypeInfo.h"
#include "infos/PlayerInfo.h"
#include "worldentities/ControllableEntity.h"
#include "worldentities/pawns/Spectator.h"
//#include "gametypes/Gametype.h"

namespace orxonox
{
    CreateFactory(GametypeStatus);

    static const std::string __CC_GametypeStatus_name = "GametypeStatus";
    static const std::string __CC_displayCaption_name = "displayCaption";

    SetConsoleCommand(__CC_GametypeStatus_name, __CC_displayCaption_name, &GametypeStatus::setDisplayCaption);

    GametypeStatus::GametypeStatus(BaseObject* creator) : OverlayText(creator)
    {
        RegisterObject(GametypeStatus);

        //this->game_ = 0;
        this->owner_ = 0;
        this->bNoCaption_ = false;
        //this->bForcedSpawn_ = false;

        ModifyConsoleCommand(__CC_GametypeStatus_name, __CC_displayCaption_name).setObject(this);
    }

    GametypeStatus::~GametypeStatus()
    {
        ModifyConsoleCommand(__CC_GametypeStatus_name, __CC_displayCaption_name).setObject(0);
    }

    void GametypeStatus::tick(float dt)
    {
        SUPER(GametypeStatus, tick, dt);

        if (this->owner_ && this->owner_->getGametypeInfo() && this->owner_->getControllableEntity())
        {
            //if (this->game_)
            //    this->bForcedSpawn_ = this->game_->getForceSpawn();
            //else
            //    this->bForcedSpawn_ = false;

            const GametypeInfo* gtinfo = this->owner_->getGametypeInfo();
            ControllableEntity* ce = this->owner_->getControllableEntity();

            if (this->bNoCaption_) // No captions are displayed.
            {
                this->setCaption("");
                return;
            }

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
                else if (ce->isA(Class(Spectator))/*&&(!bForcedSpawn_)*/)
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
        //this->game_ = orxonox_cast<Gametype*>(this->getOwner());
        this->owner_ = orxonox_cast<PlayerInfo*>(this->getOwner());
    }

    /**
    @brief
        Sets whether the gametype status is displayed.
    @param bValue
        If true captions are displayed, if false, not.
    */
    void GametypeStatus::setDisplayCaption(bool bValue)
    {
        this->bNoCaption_ = !bValue;
    }

}
