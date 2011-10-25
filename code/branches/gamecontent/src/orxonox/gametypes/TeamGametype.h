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

#ifndef _TeamGametype_H__
#define _TeamGametype_H__

#include "OrxonoxPrereqs.h"

#include <map>
#include <vector>
#include "Gametype.h"

namespace orxonox
{
    class _OrxonoxExport TeamGametype : public Gametype
    {
        public:
            TeamGametype(BaseObject* creator);
            virtual ~TeamGametype() {}

            void setConfigValues();

            virtual void playerEntered(PlayerInfo* player);
            virtual void findAndSetTeam(PlayerInfo* player);
            virtual bool playerLeft(PlayerInfo* player);

            virtual bool allowPawnHit(Pawn* victim, Pawn* originator = 0);
            virtual bool allowPawnDamage(Pawn* victim, Pawn* originator = 0);
            virtual bool allowPawnDeath(Pawn* victim, Pawn* originator = 0);

            virtual void playerStartsControllingPawn(PlayerInfo* player, Pawn* pawn);

            int getTeam(PlayerInfo* player);

            inline const ColourValue& getTeamColour(int teamnr) const
                { return this->teamcolours_[teamnr]; }

        protected:
            virtual SpawnPoint* getBestSpawnPoint(PlayerInfo* player) const;
            bool pawnsAreInTheSameTeam(Pawn* pawn1, Pawn* pawn2);

            std::map<PlayerInfo*, int> teamnumbers_;
            std::vector<ColourValue> teamcolours_;
            unsigned int teams_;
    };
}

#endif /* _TeamGametype_H__ */
