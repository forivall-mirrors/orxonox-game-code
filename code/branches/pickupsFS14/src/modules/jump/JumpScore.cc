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
 *      Fabien Vultier
 *   Co-authors:
 *      ...
 *
 */

/**
    @file JumpScore.cc
    @brief HUD of thejump minigame. If showScore_ is set, it displays the score. If showMessages_ is set, it displays the game over message.
*/

#include "JumpScore.h"
#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "util/Convert.h"
#include "infos/PlayerInfo.h"
#include "Jump.h"
#include "sound/WorldSound.h"

namespace orxonox
{
    RegisterClass(JumpScore);

    JumpScore::JumpScore(Context* context) : OverlayText(context)
    {
        RegisterObject(JumpScore);

        owner_ = NULL;
        showScore_ = false;
        showFuel_ = false;
        showMessages_ = false;
    }

    JumpScore::~JumpScore()
    {

    }

    void JumpScore::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(JumpScore, XMLPort, xmlelement, mode);

        XMLPortParam(JumpScore, "showScore", setShowScore, getShowScore, xmlelement, mode);
        XMLPortParam(JumpScore, "showFuel", setShowFuel, getShowFuel, xmlelement, mode);
        XMLPortParam(JumpScore, "showMessages", setShowMessages, getShowMessages, xmlelement, mode);
        XMLPortParam(JumpScore, "gameOverText", setGameOverText, getGameOverText, xmlelement, mode);
    }

    void JumpScore::tick(float dt)
    {
        SUPER(JumpScore, tick, dt);

        if (owner_ != NULL)
        {
            if (!owner_->hasEnded())
            {
                player_ = owner_->getPlayer();

                if (player_ != NULL)
                {
                	if (showScore_ == true)
                	{
                    	int score = owner_->getScore(player_);

                    	std::string str = multi_cast<std::string>(score);
                    	setCaption(str);
                	}
                    else if (showFuel_ == true)
                    {
                        float fuel = owner_->getFuel();
                        orxout() <<  fuel << endl;
                        std::string str;

                        if (fuel <= 0.0)
                        {
                            str = "";
                        }
                        else
                        {
                            str = multi_cast<std::string>(fuel);
                        }
                        setCaption(str);
                    }                    
                	else if (showMessages_ == true)
                	{
                    	setCaption(owner_->getDead(player_) == true ? gameOverText_ : "");
                	}
                }
            }
        }
    }

    void JumpScore::changedOwner()
    {
        SUPER(JumpScore, changedOwner);

        if (this->getOwner() != NULL && this->getOwner()->getGametype())
        {
            this->owner_ = orxonox_cast<Jump*>(this->getOwner()->getGametype().get());
        }
        else
        {
            this->owner_ = NULL;
        }
    }
}
