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


#include "TeamBaseMatchBase.h"

#include "core/CoreIncludes.h"
#include "interfaces/PawnListener.h"
#include "interfaces/TeamColourable.h"
#include "objects/gametypes/TeamBaseMatch.h"

namespace orxonox
{
    CreateFactory(TeamBaseMatchBase);

    TeamBaseMatchBase::TeamBaseMatchBase(BaseObject* creator) : Pawn(creator)
    {
        RegisterObject(TeamBaseMatchBase);

        this->state_ = BaseState::uncontrolled;

        TeamBaseMatch* gametype = orxonox_cast<TeamBaseMatch*>(this->getGametype());
        if (gametype)
        {
            gametype->addBase(this);
        }

        this->setRadarObjectShape(RadarViewable::Triangle);
    }

    void TeamBaseMatchBase::changeTeamColour()
    {
        this->fireEvent();

        TeamDeathmatch* gametype = orxonox_cast<TeamDeathmatch*>(this->getGametype());
        if (!gametype)
            return;

        ColourValue colour;

        switch (this->state_)
        {
            case BaseState::controlTeam1:
                colour = gametype->getTeamColour(0);
                break;
            case BaseState::controlTeam2:
                colour = gametype->getTeamColour(1);
                break;
            case BaseState::uncontrolled:
            default:
                colour = ColourValue(0.5, 0.5, 0.5, 1.0);
                break;
        }


        std::set<WorldEntity*> attachments = this->getAttachedObjects();
        for (std::set<WorldEntity*>::iterator it = attachments.begin(); it != attachments.end(); ++it)
        {
            if ((*it)->isA(Class(TeamColourable)))
            {
                TeamColourable* tc = orxonox_cast<TeamColourable*>(*it);
                tc->setTeamColour(colour);
            }
        }

        this->setRadarObjectColour(colour);

        // Call this so bots stop shooting at the base after they converted it
        for (ObjectList<PawnListener>::iterator it = ObjectList<PawnListener>::begin(); it != ObjectList<PawnListener>::end(); ++it)
            it->destroyedPawn(this);
    }
}

