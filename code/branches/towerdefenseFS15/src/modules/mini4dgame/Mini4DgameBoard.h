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
    @file Mini4DgameBoard.h
    @brief Declaration of the Mini4DgameBoard class.
    @ingroup Pong
*/

#ifndef _Mini4DgameBoard_H__
#define _Mini4DgameBoard_H__

#include "mini4dgame/Mini4DgamePrereqs.h"

#include "worldentities/StaticEntity.h"

namespace orxonox
{

    struct Mini4DgamePosition
    {
        Mini4DgamePosition(int x, int y, int z, int w) : x(x), y(y), z(z), w(w) {}
        int x;
        int y;
        int z;
        int w;
    };

    struct Mini4DgameWinner
    {
        int winningRow[4];
        int winningColumn[4];
        int winningHeight[4];
        int winningNumber[4];
        int color_;
    };

    /**
    @brief
        This class manages the board for @ref orxonox::Mini4Dgame "Mini4Dgame".

    @author
        Oliver Richter
    */
    class _Mini4DgameExport Mini4DgameBoard : public StaticEntity
    {
        public:
            Mini4DgameBoard(Context* context);
            //virtual ~Mini4DgameBoard();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            bool isValidMove(const Mini4DgamePosition& move);
            void undoMove();
            void makeMove(const Mini4DgamePosition& move);
            Mini4DgameWinner getWinner();

            void changedGametype();
            void checkGametype();

        private:
            //void registerVariables();
            std::vector<Mini4DgamePosition> moves;
            bool player_toggle_;
            BlinkingBillboard* blinkingBillboards[4][4][4][4];
            int board[4][4][4][4]; //!< The logical board where the game takes place. board[row][column][height][number]
    };
}

#endif /* _Mini4DgameBoard_H__ */
