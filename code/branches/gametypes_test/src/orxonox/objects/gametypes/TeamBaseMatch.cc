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

#include "TeamBaseMatch.h"

#include "objects/worldentities/pawns/TeamBaseMatchBase.h"
#include "core/CoreIncludes.h"

namespace orxonox
{
    CreateUnloadableFactory(TeamBaseMatch);

    TeamBaseMatch::TeamBaseMatch(BaseObject* creator) : TeamDeathmatch(creator)
    {
        RegisterObject(TeamBaseMatch);

        this->scoreTimer_.setTimer(10, true, this, createExecutor(createFunctor(&TeamBaseMatch::winPoints)));
        this->outputTimer_.setTimer(30, true, this, createExecutor(createFunctor(&TeamBaseMatch::showPoints)));

        this->pointsTeam1_ = 0;
        this->pointsTeam2_ = 0;
    }

    // Change the control of the defeated base and respawn it with its initial health
    bool TeamBaseMatch::allowPawnDeath(Pawn* victim, Pawn* originator)
    {
        TeamBaseMatchBase* base = dynamic_cast<TeamBaseMatchBase*>(victim);
        if (base)
        {
            std::set<TeamBaseMatchBase*>::const_iterator it = this->bases_.find(base);
            if (it != this->bases_.end())
            {
                int teamnr = this->getTeam(originator->getPlayer());
                if (teamnr == 0)
                    base->setState(BaseState::controlTeam1);
                if (teamnr == 1)
                    base->setState(BaseState::controlTeam2);
            }

            victim->setHealth(victim->getInitialHealth());
            return false;
        }

        return TeamDeathmatch::allowPawnDeath(victim, originator);
    }


    // if the player is in the same team as the base, he can't make any damage to it
    bool TeamBaseMatch::allowPawnDamage(Pawn* victim, Pawn* originator)
    {
        TeamBaseMatchBase* base = dynamic_cast<TeamBaseMatchBase*>(victim);
        if (base)
        {
            std::set<TeamBaseMatchBase*>::const_iterator it = this->bases_.find(base);
            if (it != this->bases_.end())
                return (!this->pawnsAreInTheSameTeam(victim, base));
        }
        return (!this->pawnsAreInTheSameTeam(victim, originator));
    }

    bool TeamBaseMatch::pawnsAreInTheSameTeam(Pawn* pawn1, TeamBaseMatchBase* base)
    {
        if (pawn1 && base)
        {
            std::map<PlayerInfo*, int>::const_iterator it1 = this->teamnumbers_.find(pawn1->getPlayer());
            int teamnrbase = -1;
            int teamnrplayer = getTeam(pawn1->getPlayer());

            switch (base->getState())
            {
                case BaseState::controlTeam1:
                    teamnrbase = 0;
                    break;
                case BaseState::controlTeam2:
                    teamnrbase = 1;
                    break;
                case BaseState::uncontrolled:
                default:
                    teamnrbase = -1;
            }

            if (teamnrbase == teamnrplayer)
                return false;
        }
        return true;
    }





    // collect Points for killing oppenents
    void TeamBaseMatch::playerScored(PlayerInfo* player)
    {
        int teamnr = this->getTeam(player);
        this->addTeamPoints(teamnr, 5);
    }

    // show points or each interval of time
    void TeamBaseMatch::showPoints()
    {

	COUT(0) << "Points standing:" << std::endl << "Team 1: "<< pointsTeam1_ << std::endl << "Team 2: " << pointsTeam2_ << std::endl;
	if(pointsTeam1_ >=1700) COUT(0) << "Team 1 is near victory!" << std::endl;
	if(pointsTeam2_ >=1700) COUT(0) << "Team 2 is near victory!" << std::endl;
    }


    // collect Points while controlling Bases
    void TeamBaseMatch::winPoints()
    {
	int amountControlled = 0;
	int amountControlled2 = 0;

        for (std::set<TeamBaseMatchBase*>::const_iterator it = this->bases_.begin(); it != this->bases_.end(); ++it)
        {
	    if((*it)->getState() == BaseState::controlTeam1)
	    {
	        amountControlled++;
	    }
	    if((*it)->getState() == BaseState::controlTeam2)
	    {
		amountControlled2++;
	    }
        }

        this->addTeamPoints(0, (amountControlled * 30));
        this->addTeamPoints(1, (amountControlled2 * 30));
    }


    // end game if one team reaches 2000 points
    void TeamBaseMatch::endGame()
    {
        if(this->pointsTeam1_>=2000 || this->pointsTeam2_ >=2000)
        {
            this->end();
        }
    }


    // this function is called by the function winPoints() which adds points to the teams for every base and killed openents at a certain time
    void TeamBaseMatch::addTeamPoints(int team, int points)
    {
        if(team == 0)
        {
            this->pointsTeam1_ += points;
        }
        if(team == 1)
        {
            this->pointsTeam2_ += points;
        }

        this->endGame();
    }

    void TeamBaseMatch::addBase(TeamBaseMatchBase* base)
    {
        this->bases_.insert(base);
        base->setState(BaseState::uncontrolled);
    }

    TeamBaseMatchBase* TeamBaseMatch::getBase(unsigned int index) const
    {
        unsigned int i = 0;
        for (std::set<TeamBaseMatchBase*>::const_iterator it = this->bases_.begin(); it != this->bases_.end(); ++it)
        {
            i++;
            if (i > index)
                return (*it);
        }
        return 0;
    }

}


