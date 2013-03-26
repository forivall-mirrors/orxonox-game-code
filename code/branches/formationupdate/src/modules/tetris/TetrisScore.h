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
    @file TetrisScore.h
    @brief Declaration of the TetrisScore class.
    @ingroup Tetris
*/

#ifndef _TetrisScore_H__
#define _TetrisScore_H__

#include "tetris/TetrisPrereqs.h"

#include "tools/interfaces/Tickable.h"

#include "overlays/OverlayText.h"

namespace orxonox
{

    /**
    @brief
        The TetrisScore class displays the score for a game of @ref orxonox::Tetris "Tetris".

    @author
        Johannes Ritz

    @ingroup Tetris
    */
    class _TetrisExport TetrisScore : public OverlayText, public Tickable
    {
        public:
            TetrisScore(BaseObject* creator);
            virtual ~TetrisScore();

            virtual void tick(float dt); //!< Creates and sets the caption to be displayed by the TetrisScore.
            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            virtual void changedOwner(); //!< Is called when the owner changes.

        private:
            Tetris* owner_; //!< The Tetris game that owns this TetrisScore.
            PlayerInfo* player_; //!< Store information about the player permanently.
    };
}
#endif /* _TetrisScore_H__ */
