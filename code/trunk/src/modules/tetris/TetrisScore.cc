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
 *      ...
 *
 */

/**
    @file TetrisScore.cc
    @brief Implementation of the TetrisScore class.
    @ingroup Tetris
*/

#include "TetrisScore.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "util/Convert.h"

#include "infos/PlayerInfo.h"

#include "Tetris.h"

namespace orxonox
{
    RegisterClass(TetrisScore);

    /**
    @brief
        Constructor. Registers and initializes the object.
    @ingroup Tetris
    */
    TetrisScore::TetrisScore(Context* context) : OverlayText(context)
    {
        RegisterObject(TetrisScore);

        this->owner_ = 0;
        this->player_ = NULL;
    }

    /**
    @brief
        Destructor.
    */
    TetrisScore::~TetrisScore()
    {
    }

    /**
    @brief
        Method to create a TetrisScore through XML.
    */
    void TetrisScore::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(TetrisScore, XMLPort, xmlelement, mode);
    }

    /**
    @brief
        Is called each tick.
        Creates and sets the caption to be displayed by the TetrisScore.
    @param dt
        The time that has elapsed since the last tick.
    */
    void TetrisScore::tick(float dt)
    {
        SUPER(TetrisScore, tick, dt);

        // If the owner is set. The owner being a Tetris game.
        if (this->owner_ != NULL)
        {
            std::string score("0");
	        if(!this->owner_->hasEnded())
            {
                //get the player
                player_ = this->owner_->getPlayer();
            }

            if(this->owner_->hasStarted())
            {
                // Save the name and score of each player as a string.
                if (player_ != NULL)
                    score = multi_cast<std::string>(this->owner_->getScore(player_));
            }
            this->setCaption(score);
        }
    }

    /**
    @brief
        Is called when the owner changes.
        Sets the owner to NULL, if it is not a pointer to a Tetris game.
    */
    void TetrisScore::changedOwner()
    {
        SUPER(TetrisScore, changedOwner);

        if (this->getOwner() != NULL && this->getOwner()->getGametype())
            this->owner_ = orxonox_cast<Tetris*>(this->getOwner()->getGametype().get());
        else
            this->owner_ = 0;
    }
}
