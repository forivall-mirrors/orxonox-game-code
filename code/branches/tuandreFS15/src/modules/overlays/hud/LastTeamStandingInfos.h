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
 *      
 *
 */

#ifndef _LastTeamStandingInfos_H__
#define _LastTeamStandingInfos_H__

#include "overlays/OverlaysPrereqs.h"

#include "tools/interfaces/Tickable.h"
#include "overlays/OverlayText.h"

namespace orxonox
{
    class _OverlaysExport LastTeamStandingInfos : public OverlayText, public Tickable
    {
        public:
            LastTeamStandingInfos(Context* context);
            virtual ~LastTeamStandingInfos();

            virtual void tick(float dt);
            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            virtual void changedOwner();

            inline void setShowLives(bool value)
                { this->bShowLives_ = value; }
            inline bool getShowLives() const
                { return this->bShowLives_; }

            inline void setShowTeams(bool value)
                { this->bShowTeams_ = value; }
            inline bool getShowTeams() const
                { return this->bShowTeams_; }

        private:
            LastTeamStanding* lts_;
            PlayerInfo* player_;
            bool bShowLives_;
            bool bShowTeams_;
    };
}
#endif /* _LastTeamStandingInfos_H__ */
