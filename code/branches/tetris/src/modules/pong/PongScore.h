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
    
    */
    class _PongExport PongScore : public OverlayText, public Tickable
    {
        public:
            PongScore(BaseObject* creator);
            virtual ~PongScore();

            virtual void tick(float dt);
            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            virtual void changedOwner();

            inline void setShowName(bool value)
                { this->bShowName_ = value; }
            inline bool getShowName() const
                { return this->bShowName_; }

            inline void setShowScore(bool value)
                { this->bShowScore_ = value; }
            inline bool getShowScore() const
                { return this->bShowScore_; }

            inline void setShowLeftPlayer(bool value)
                { this->bShowLeftPlayer_ = value; }
            inline bool getShowLeftPlayer() const
                { return this->bShowLeftPlayer_; }

            inline void setShowRightPlayer(bool value)
                { this->bShowRightPlayer_ = value; }
            inline bool getShowRightPlayer() const
                { return this->bShowRightPlayer_; }

        private:
            Pong* owner_;
            bool bShowName_;
            bool bShowScore_;
            bool bShowLeftPlayer_;
            bool bShowRightPlayer_;
    };
}
#endif /* _PongScore_H__ */
