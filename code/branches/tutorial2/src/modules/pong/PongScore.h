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
    @file PongScore.h
    @brief Declaration of the PongScore class.
    @ingroup Pong
*/

#ifndef _PongScore_H__
#define _PongScore_H__

#include "pong/PongPrereqs.h"

#include "tools/interfaces/Tickable.h"

#include "overlays/OverlayText.h"

namespace orxonox
{

    /**
    @brief
        The PongScore class displays the score for a game of @ref orxonox::Pong "Pong".

    @author
        Fabian 'x3n' Landau

    @ingroup Pong
    */
    class _PongExport PongScore : public OverlayText, public Tickable
    {
        public:
            PongScore(BaseObject* creator);
            virtual ~PongScore();

            virtual void tick(float dt); //!< Creates and sets the caption to be displayed by the PongScore.
            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            virtual void changedOwner(); //!< Is called when the owner changes.

            /**
            @brief Set whether the PongScore displays the players' names.
            @param value If true the players' names are displayed.
            */
            inline void setShowName(bool value)
                { this->bShowName_ = value; }
            /**
            @brief Get whether the PongScore displays the players' names.
            @return Returns true if the players' names are displayed, false otherwise.
            */
            inline bool getShowName() const
                { return this->bShowName_; }

            /**
            @brief Set whether the PongScore displays the players' scores.
            @param value If true the players' scores are displayed.
            */
            inline void setShowScore(bool value)
                { this->bShowScore_ = value; }
            /**
            @brief Get whether the PongScore displays the players' scores.
            @return Returns true if the players' scores are displayed, false otherwise.
            */
            inline bool getShowScore() const
                { return this->bShowScore_; }

            /**
            @brief Set whether the PongScore displays the left player.
            @param value If true the left player is displayed.
            */
            inline void setShowLeftPlayer(bool value)
                { this->bShowLeftPlayer_ = value; }
            /**
            @brief Get whether the PongScore displays the left player.
            @return Returns true if the left player is displayed, false otherwise.
            */
            inline bool getShowLeftPlayer() const
                { return this->bShowLeftPlayer_; }

            /**
            @brief Set whether the PongScore displays the right player.
            @param value If true the right player is displayed.
            */
            inline void setShowRightPlayer(bool value)
                { this->bShowRightPlayer_ = value; }
            /**
            @brief Get whether the PongScore displays the right player.
            @return Returns true if the right player is displayed, false otherwise.
            */
            inline bool getShowRightPlayer() const
                { return this->bShowRightPlayer_; }

        private:
            Pong* owner_; //!< The Pong game that owns this PongScore.
            bool bShowName_; //!< Whether the names of the players are shown.
            bool bShowScore_; //!< Whether the score of the players is shown.
            bool bShowLeftPlayer_; //!< Whether the left player is shown.
            bool bShowRightPlayer_; //!< Whether the right player is shown.
            PlayerInfo* player1_; //!< Store information about left player permanently.
            PlayerInfo* player2_; //!< Same for the right player. To end the game properly.
    };
}
#endif /* _PongScore_H__ */
