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

#include "OrxonoxStableHeaders.h"
#include "Scoreboard.h"

#include <string>
#include <OgreOverlay.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OgreOverlayContainer.h>

#include "util/Convert.h"
#include "util/Debug.h"
#include "core/CoreIncludes.h"
#include "core/ConfigValueIncludes.h"
#include "objects/gametypes/Gametype.h"
#include "CreateLines.h"

namespace orxonox
{
    CreateFactory(Scoreboard);

    /**
        @brief Constructor: Creates the scoreboard.
    */
    Scoreboard::Scoreboard(BaseObject* creator)
        : OrxonoxOverlay(creator)
    {
        RegisterObject(Scoreboard);
    }

    /**
        @brief Prints the scoreboard on the screen.
    */
    void Scoreboard::printLines() {

        numberOfColumns = 2;
        numberOfLines = this->gametype_->getNumberOfPlayers();
        columnIndex = 0;
        topOffset = 0.3;
        lineSpacing = 0.1;

        for (unsigned int i = 0; i < numberOfLines; i++) {

            leftOffset = 0.3;

            this->createlines_->setNumberOfColumns(numberOfColumns, i);

            columnText = this->gametype_->getPlayersName();
            this->createlines_->alignColumn(columnIndex, topOffset, leftOffset);
            this->createlines_->setColumnText(columnIndex, columnText);

            columnIndex++;
            leftOffset = leftOffset + 0.4;

            columnText = this->gametype_->getPlayersFrags();
            this->createlines_->alignColumn(columnIndex, topOffset, leftOffset);
            this->createlines_->setColumnText(columnIndex, columnText);

            topOffset = topOffset + lineSpacing;

        }

    }

}
