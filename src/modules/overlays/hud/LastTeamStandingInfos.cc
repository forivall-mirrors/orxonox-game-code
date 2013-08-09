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
 *      
 *
 */

#include "LastTeamStandingInfos.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "util/Convert.h"
#include "gametypes/LastTeamStanding.h"
#include "infos/PlayerInfo.h"

namespace orxonox
{
    CreateFactory(LastTeamStandingInfos);

    LastTeamStandingInfos::LastTeamStandingInfos(Context* context) : OverlayText(context)
    {
        RegisterObject(LastTeamStandingInfos);

        this->lts_ = 0;
        this->player_ = 0;
        this->bShowLives_ = false;
        this->bShowTeams_ = false;
    }

    LastTeamStandingInfos::~LastTeamStandingInfos()
    {
    }

    void LastTeamStandingInfos::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(LastTeamStandingInfos, XMLPort, xmlelement, mode);

        XMLPortParam(LastTeamStandingInfos, "showlives",     setShowLives,     getShowLives,     xmlelement, mode).defaultValues(false);
        XMLPortParam(LastTeamStandingInfos, "showTeams",     setShowTeams,     getShowTeams,     xmlelement, mode).defaultValues(false);
    }

    void LastTeamStandingInfos::tick(float dt)
    {
        SUPER(LastTeamStandingInfos, tick, dt);

        if (this->player_ && this->lts_)
        {
            if (this->bShowLives_)
            {
                const std::string& lives = multi_cast<std::string>(this->lts_->playerGetLives(this->player_));
                this->setCaption(lives);
            }
            else if(this->bShowTeams_)
            {
                const std::string& Teams = multi_cast<std::string>(this->lts_->getNumTeamsAlive());
                this->setCaption(Teams);
            }
        }
    }

    void LastTeamStandingInfos::changedOwner()
    {
        SUPER(LastTeamStandingInfos, changedOwner);

        if (this->getOwner() && this->getOwner()->getGametype())
        {
            this->player_ = orxonox_cast<PlayerInfo*>(this->getOwner());
            this->lts_ = orxonox_cast<LastTeamStanding*>(this->getOwner()->getGametype().get());
        }
        else
        {
            this->player_ = 0;
            this->lts_ = 0;
        }
    }
}
