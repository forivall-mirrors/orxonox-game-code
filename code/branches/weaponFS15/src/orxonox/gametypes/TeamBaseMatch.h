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
 *      Val Mikos
 *   Co-authors:
 *      ...
 *
 */

#ifndef _TeamBaseMatch_H__
#define _TeamBaseMatch_H__

#include "OrxonoxPrereqs.h"

#include <set>
#include "tools/Timer.h"
#include "TeamDeathmatch.h"

namespace orxonox
{
    class _OrxonoxExport TeamBaseMatch : public TeamDeathmatch
    {
        public:
            TeamBaseMatch(Context* context);
            virtual ~TeamBaseMatch() {}

            virtual bool allowPawnDeath(Pawn* victim, Pawn* originator = 0);
            virtual bool allowPawnDamage(Pawn* victim, Pawn* originator);

            virtual void playerScored(PlayerInfo* player, int score = 1);
            virtual void showPoints();
            virtual void endGame();

            void addBase(TeamBaseMatchBase* base);
            TeamBaseMatchBase* getBase(unsigned int index) const;

            void addTeamPoints(int team, int points);
            int getTeamPoints(int team);
            int getTeamBases(int team);

        protected:
            void winPoints();

            bool pawnsAreInTheSameTeam(Pawn* pawn1, TeamBaseMatchBase* base);
            using TeamDeathmatch::pawnsAreInTheSameTeam;

            std::set<TeamBaseMatchBase*> bases_;
            Timer scoreTimer_;
            Timer outputTimer_;

            //points for each team
            int pointsTeam1_;
            int pointsTeam2_;
    };
}

#endif /* _TeamBaseMatch_H__ */
