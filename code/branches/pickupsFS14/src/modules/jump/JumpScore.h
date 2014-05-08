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
    @file JumpScore.h
    @brief Declaration of the JumpScore class.
    @ingroup Jump
*/

#ifndef _JumpScore_H__
#define _JumpScore_H__

#include "jump/JumpPrereqs.h"

#include "tools/interfaces/Tickable.h"

#include "overlays/OverlayText.h"

namespace orxonox
{

    /**
    @brief
        The JumpScore class displays the score for a game of @ref orxonox::Jump "Jump".

    @author
        Fabian 'x3n' Landau

    @ingroup Jump
    */
    class _JumpExport JumpScore : public OverlayText, public Tickable
    {
        public:
            JumpScore(Context* context);
            virtual ~JumpScore();

            virtual void tick(float dt); //!< Creates and sets the caption to be displayed by the JumpScore.
            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            virtual void changedOwner(); //!< Is called when the owner changes.

        private:
            Jump* owner_; //!< The Jump game that owns this JumpScore.
            WeakPtr<PlayerInfo> player_; //!< Store information about left player permanently.
            WorldSound* scoreSound_;

    };
}
#endif /* _JumpScore_H__ */
