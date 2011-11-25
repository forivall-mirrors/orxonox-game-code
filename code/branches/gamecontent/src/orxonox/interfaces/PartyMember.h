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
 *      Damian 'Mozork' Frick
 *   Co-authors:
 *      ...
 *
 */

/**
    @file
    @brief Definition of the PartyMember class.
*/

#ifndef _PartyMember_H__
#define _PartyMember_H__

#include "OrxonoxPrereqs.h"
#include "core/OrxonoxClass.h"

namespace orxonox
{
    /**
    @brief
        PartyMember is an Interface, that can be implemented by any object to enable it to belong to a certain group. (friend, foe, neutral, ...)

        It just needs to inherit form PartyMember, and implement the  methods.
    @author
        Damian 'Mozork' Frick
    */
    class _OrxonoxExport PartyMember : virtual public OrxonoxClass
    {
        public:
            PartyMember();
            virtual ~PartyMember();



            /**
            @brief
                Method to set the objects teamnumber.
                Must be implemented by every class inheriting from PartyMember. It is virtual in order to allow to implement a hirarchy.
                Hirarchy: PlayerInfo > Controller > Pawn. When the level is set in the controller it should be also set in the controlled pawn.
            @param team The teamnumber to which a PartyMember belongs to. Value '-1' means no team is set. 0,1,2, ... are legal team numbers.

            @return

            */
            virtual inline void setTeam(int team) =0;

            inline int getTeam() const
                { return this->team_; }

        protected:
            int team_; //<! Teamnumber: to identify to which group a PartyMember belongs to
                       //<! Default value: -1 : neutral the meaning of other values is defined in the context


    };
}

#endif /* _PartyMember_H__ */
