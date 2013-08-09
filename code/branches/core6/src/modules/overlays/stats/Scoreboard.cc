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
 *      Benjamin Hildebrandt
 *
 */

#include "Scoreboard.h"

#include "util/Convert.h"
#include "core/CoreIncludes.h"
#include "gametypes/Gametype.h"
#include "infos/PlayerInfo.h"
#include "CreateLines.h"

namespace orxonox
{
    CreateFactory(Scoreboard);

    /**
        @brief Constructor: Creates the scoreboard.
    */
    Scoreboard::Scoreboard(Context* context)
        : OrxonoxOverlay(context)
    {
        RegisterObject(Scoreboard);
    }

    Scoreboard::~Scoreboard()
    {
        while (this->lines_.size() > 0)
        {
            // destroy lines
            delete this->lines_.back();
            this->lines_.pop_back();
        }
    }

    void Scoreboard::changedVisibility()
    {
        SUPER(Scoreboard, changedVisibility);

        for (unsigned int i = 0; i < this->lines_.size(); ++i)
            this->lines_[i]->changedVisibility();
    }

    /**
        @brief Prints the scoreboard on the screen.
    */
    void Scoreboard::tick(float dt)
    {
        const std::map<PlayerInfo*, Player>& playerList = this->getGametype()->getPlayers();

        const float topOffset  = 0.2f;
        const float leftOffset = 0.075f;
        const float distance   = 0.01f;
        const float width      = 0.85f;
        const float height     = 0.05f;
        while (playerList.size() > this->lines_.size())
        {
            // create new lines
            CreateLines* lines = new CreateLines(leftOffset, topOffset + (distance + height) * lines_.size(), width, height);
            lines->setVisibility(this->isVisible());
            lines->setOverlayGroup( this->getOverlayGroup() );
            this->lines_.push_back(lines);
        }
        while (playerList.size() < this->lines_.size())
        {
            // destroy lines
            delete this->lines_.back();
            this->lines_.pop_back();
        }

        // update board

        unsigned int index = 0;
        for (std::map<PlayerInfo*, Player>::const_iterator it = playerList.begin(); it != playerList.end(); ++it)
        {
            this->lines_[index]->setPlayerName(multi_cast<std::string>(it->first->getName()));
            this->lines_[index]->setScore(multi_cast<std::string>(it->second.frags_));
            this->lines_[index]->setDeaths(multi_cast<std::string>(it->second.killed_));
            index++;
        }

        //numberOfColumns = 2;
        //numberOfLines = 3;
        //topOffset = 0.1;
        //lineSpacing = 0.1;

        //for (unsigned int i = 0; i < this->lines_.size(); ++i)
        //{
        //    columnIndex = 0;
        //    leftOffset = 0.1;

        //    this->createlines_->setNumberOfColumns(numberOfColumns, i);

        //    // columnText = this->getGametype()->getPlayersName();
        //    columnText = "PlayerName";
        //    this->createlines_->alignColumn(columnIndex, topOffset, leftOffset);
        //    this->createlines_->setColumnText(columnIndex, columnText);

        //    columnIndex++;
        //    leftOffset = leftOffset + 0.3;

        //    // columnText = this->getGametype()->getPlayersFrags();
        //    columnText = "PlayerFrags";
        //    this->createlines_->alignColumn(columnIndex, topOffset, leftOffset);
        //    this->createlines_->setColumnText(columnIndex, columnText);

        //    topOffset = topOffset + lineSpacing;
        //}
    }
}
