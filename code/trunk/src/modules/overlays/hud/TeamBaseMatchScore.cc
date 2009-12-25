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

#include "TeamBaseMatchScore.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "util/Convert.h"
#include "gametypes/TeamBaseMatch.h"
#include "infos/PlayerInfo.h"

namespace orxonox
{
    CreateFactory(TeamBaseMatchScore);

    TeamBaseMatchScore::TeamBaseMatchScore(BaseObject* creator) : OverlayText(creator)
    {
        RegisterObject(TeamBaseMatchScore);

        this->owner_ = 0;

        this->bShowBases_ = false;
        this->bShowScore_ = false;
        this->bShowLeftTeam_ = false;
        this->bShowRightTeam_ = false;
    }

    TeamBaseMatchScore::~TeamBaseMatchScore()
    {
    }

    void TeamBaseMatchScore::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(TeamBaseMatchScore, XMLPort, xmlelement, mode);

        XMLPortParam(TeamBaseMatchScore, "showbases",     setShowBases,     getShowBases,     xmlelement, mode).defaultValues(false);
        XMLPortParam(TeamBaseMatchScore, "showscore",     setShowScore,     getShowScore,     xmlelement, mode).defaultValues(false);
        XMLPortParam(TeamBaseMatchScore, "showleftteam",  setShowLeftTeam,  getShowLeftTeam,  xmlelement, mode).defaultValues(false);
        XMLPortParam(TeamBaseMatchScore, "showrightteam", setShowRightTeam, getShowRightTeam, xmlelement, mode).defaultValues(false);
    }

    void TeamBaseMatchScore::tick(float dt)
    {
        SUPER(TeamBaseMatchScore, tick, dt);

        if (this->owner_)
        {
            const std::string& bases1 = '(' + multi_cast<std::string>(this->owner_->getTeamBases(0)) + ')';
            const std::string& bases2 = '(' + multi_cast<std::string>(this->owner_->getTeamBases(1)) + ')';

            const std::string& score1 = multi_cast<std::string>(this->owner_->getTeamPoints(0));
            const std::string& score2 = multi_cast<std::string>(this->owner_->getTeamPoints(1));

            std::string output1;
            if (this->bShowLeftTeam_)
            {
                if (this->bShowBases_ && this->bShowScore_)
                    output1 = bases1 + " - " + score1;
                else if (this->bShowScore_)
                    output1 = score1;
                else if (this->bShowBases_)
                    output1 = bases1;
            }

            std::string output2;
            if (this->bShowRightTeam_)
            {
                if (this->bShowBases_ && this->bShowScore_)
                    output2 = score2 + " - " + bases2;
                else if (this->bShowScore_)
                    output2 = score2;
                else if (this->bShowBases_)
                    output2 = bases2;
            }

            std::string output;
            if (this->bShowBases_ || this->bShowScore_)
            {
                if (this->bShowLeftTeam_ && this->bShowRightTeam_)
                    output = output1 + ':' + output2;
                else if (this->bShowLeftTeam_ || this->bShowRightTeam_)
                    output = output1 + output2;
            }

            this->setCaption(output);
        }
    }


    void TeamBaseMatchScore::changedOwner()
    {
        SUPER(TeamBaseMatchScore, changedOwner);

        if (this->getOwner() && this->getOwner()->getGametype())
            this->owner_ = orxonox_cast<TeamBaseMatch*>(this->getOwner()->getGametype().get());
        else
            this->owner_ = 0;
    }
}
