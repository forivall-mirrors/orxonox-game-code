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

#include "PongScore.h"

#include "util/Convert.h"
#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "Pong.h"
#include "infos/PlayerInfo.h"

namespace orxonox
{
    CreateFactory(PongScore);

    PongScore::PongScore(BaseObject* creator) : OverlayText(creator)
    {
        RegisterObject(PongScore);

        this->owner_ = 0;

        this->bShowName_ = false;
        this->bShowScore_ = false;
        this->bShowLeftPlayer_ = false;
        this->bShowRightPlayer_ = false;
    }

    PongScore::~PongScore()
    {
    }

    void PongScore::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(PongScore, XMLPort, xmlelement, mode);

        XMLPortParam(PongScore, "showname",        setShowName,        getShowName,        xmlelement, mode).defaultValues(false);
        XMLPortParam(PongScore, "showscore",       setShowScore,       getShowScore,       xmlelement, mode).defaultValues(false);
        XMLPortParam(PongScore, "showleftplayer",  setShowLeftPlayer,  getShowLeftPlayer,  xmlelement, mode).defaultValues(false);
        XMLPortParam(PongScore, "showrightplayer", setShowRightPlayer, getShowRightPlayer, xmlelement, mode).defaultValues(false);
    }

    void PongScore::tick(float dt)
    {
        SUPER(PongScore, tick, dt);

        if (this->owner_)
        {
            PlayerInfo* player1 = this->owner_->getLeftPlayer();
            PlayerInfo* player2 = this->owner_->getRightPlayer();

            std::string name1;
            std::string name2;

            std::string score1("0");
            std::string score2("0");

            if (player1)
            {
                name1 = player1->getName();
                score1 = multi_cast<std::string>(this->owner_->getScore(player1));
            }

            if (player2)
            {
                name2 = player2->getName();
                score2 = multi_cast<std::string>(this->owner_->getScore(player2));
            }

            std::string output1;
            if (this->bShowLeftPlayer_)
            {
                if (this->bShowName_ && this->bShowScore_ && player1)
                    output1 = name1 + " - " + score1;
                else if (this->bShowScore_)
                    output1 = score1;
                else if (this->bShowName_)
                    output1 = name1;
            }

            std::string output2;
            if (this->bShowRightPlayer_)
            {
                if (this->bShowName_ && this->bShowScore_ && player2)
                    output2 = score2 + " - " + name2;
                else if (this->bShowScore_)
                    output2 = score2;
                else if (this->bShowName_)
                    output2 = name2;
            }

            std::string output("PONG");
            if (this->bShowName_ || this->bShowScore_)
            {
                if (this->bShowLeftPlayer_ && this->bShowRightPlayer_)
                    output = output1 + ':' + output2;
                else if (this->bShowLeftPlayer_ || this->bShowRightPlayer_)
                    output = output1 + output2;
            }

            this->setCaption(output);
        }
    }


    void PongScore::changedOwner()
    {
        SUPER(PongScore, changedOwner);

        if (this->getOwner() && this->getOwner()->getGametype())
            this->owner_ = orxonox_cast<Pong*>(this->getOwner()->getGametype().get());
        else
            this->owner_ = 0;
    }
}
