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
 *      Johannes Ritz
 *   Co-authors:
 *      Fabian 'x3n' Landau
 *
 */

#include "LastManStandingInfos.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "util/Convert.h"
#include "gametypes/LastManStanding.h"
#include "infos/PlayerInfo.h"

namespace orxonox
{
    CreateFactory(LastManStandingInfos);

    LastManStandingInfos::LastManStandingInfos(Context* context) : OverlayText(context)
    {
        RegisterObject(LastManStandingInfos);

        this->lms_ = 0;
        this->player_ = 0;
        this->bShowLives_ = false;
        this->bShowPlayers_ = false;
    }

    LastManStandingInfos::~LastManStandingInfos()
    {
    }

    void LastManStandingInfos::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(LastManStandingInfos, XMLPort, xmlelement, mode);

        XMLPortParam(LastManStandingInfos, "showlives",     setShowLives,     getShowLives,     xmlelement, mode).defaultValues(false);
        XMLPortParam(LastManStandingInfos, "showplayers",     setShowPlayers,     getShowPlayers,     xmlelement, mode).defaultValues(false);
    }

    void LastManStandingInfos::tick(float dt)
    {
        SUPER(LastManStandingInfos, tick, dt);

        if (this->player_ && this->lms_)
        {
            if (this->bShowLives_)
            {
                const std::string& lives = multi_cast<std::string>(this->lms_->playerGetLives(this->player_));
                this->setCaption(lives);
            }
            else if(this->bShowPlayers_)
            {
                const std::string& players = multi_cast<std::string>(this->lms_->getNumPlayersAlive());
                this->setCaption(players);
            }
        }
    }

    void LastManStandingInfos::changedOwner()
    {
        SUPER(LastManStandingInfos, changedOwner);

        if (this->getOwner() && this->getOwner()->getGametype())
        {
            this->player_ = orxonox_cast<PlayerInfo*>(this->getOwner());
            this->lms_ = orxonox_cast<LastManStanding*>(this->getOwner()->getGametype().get());
        }
        else
        {
            this->player_ = 0;
            this->lms_ = 0;
        }
    }
}
