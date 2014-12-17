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

#ifndef _JumpScore_H__
#define _JumpScore_H__

#include "jump/JumpPrereqs.h"

#include "tools/interfaces/Tickable.h"

#include "overlays/OverlayText.h"

namespace orxonox
{
    class _JumpExport JumpScore : public OverlayText, public Tickable
    {
        public:
            JumpScore(Context* context);
            virtual ~JumpScore();

            virtual void tick(float dt);
            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            virtual void changedOwner();

            void setShowScore(const bool showScore)
                { showScore_ = showScore; }

            const bool getShowScore() const
                { return showScore_; }

            void setShowFuel(const bool showFuel)
                { showFuel_ = showFuel; }

            const bool getShowFuel() const
                { return showFuel_; }

            void setShowMessages(const bool showMessages)
                { showMessages_ = showMessages; }

            const bool getShowMessages() const
                { return showMessages_; }                

            void setGameOverText(const std::string& gameOverText)
                { this->gameOverText_ = gameOverText; }

            const std::string& getGameOverText() const
                { return this->gameOverText_; }

        private:
            Jump* owner_;
            WeakPtr<PlayerInfo> player_;
            WorldSound* scoreSound_;
            bool showScore_;
            bool showFuel_;
            bool showMessages_;
            std::string gameOverText_;
    };
}
#endif /* _JumpScore_H__ */
