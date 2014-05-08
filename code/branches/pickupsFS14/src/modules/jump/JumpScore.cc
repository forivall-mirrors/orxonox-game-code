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

/**
    @file JumpScore.cc
    @brief Implementation of the JumpScore class.
*/

#include "JumpScore.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "util/Convert.h"

#include "infos/PlayerInfo.h"

#include "Jump.h"
#include "sound/WorldSound.h" /////////////////////////////

namespace orxonox
{
    RegisterClass(JumpScore);

    /**
    @brief
        Constructor. Registers and initializes the object.
    */
    JumpScore::JumpScore(Context* context) : OverlayText(context)
    {
        RegisterObject(JumpScore);

        this->owner_ = NULL;

    }

    /**
    @brief
        Destructor.
    */
    JumpScore::~JumpScore()
    {

    }

    /**
    @brief
        Method to create a JumpScore through XML.
    */
    void JumpScore::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(JumpScore, XMLPort, xmlelement, mode);
    }

    /**
    @brief
        Is called each tick.
        Creates and sets the caption to be displayed by the JumpScore.
    @param dt
        The time that has elapsed since the last tick.
    */
    void JumpScore::tick(float dt)
    {
        SUPER(JumpScore, tick, dt);

        // If the owner is set. The owner being a Jump game.

        if (this->owner_ != NULL)
        {
        	/*
            if (!this->owner_->hasEnded())
            {
                // Get the two players.
                player1_ = this->owner_->getPlayer();
            }

            std::string name1;

            std::string score1("0");

            // Save the name and score of each player as a string.
            if (player1_ != NULL)
            {
                name1 = player1_->getName();
                score1 = multi_cast<std::string>(this->owner_->getScore(player1_));
            }

            // Assemble the strings, depending on what should all be displayed.
            std::string output1;
            if (this->bShowLeftPlayer_)
            {
                if (this->bShowName_ && this->bShowScore_ && player1_ != NULL)
                    output1 = name1 + " - " + score1;
                else if (this->bShowScore_)
                    output1 = score1;
                else if (this->bShowName_)
                    output1 = name1;
            }

            std::string output("JUMP");
            if (this->bShowName_ || this->bShowScore_)
            {
                if (this->bShowLeftPlayer_ && this->bShowRightPlayer_)
                    output = output1 + ':' + output2;
                else if (this->bShowLeftPlayer_ || this->bShowRightPlayer_)
                    output = output1 + output2;
            }

            this->setCaption(output);*/




            if (!owner_->hasEnded())
            {
                player_ = owner_->getPlayer();

                if (player_ != NULL)
                {
                	int score = owner_->getScore(player_);

                	std::string str = multi_cast<std::string>(score);
                	setCaption(str);
                }
            }
        }
    }

    /**
    @brief
        Is called when the owner changes.
        Sets the owner to NULL, if it is not a pointer to a Jump game.
    */
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
