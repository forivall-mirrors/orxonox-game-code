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

#ifndef _Rewardable_H__
#define _Rewardable_H__

#include "core/OrxonoxClass.h"

namespace orxonox {

    /**
    @brief
        Rewardable is an Interface, that can be implemented by any object to enable it to be given as reward to a player through QuestEffects.
    @author
        Damian 'Mozork' Frick
    */
    class Rewardable : virtual public OrxonoxClass, public BaseObject
    {
    
	public:
	    Rewardable();
	    ~Rewardable();
	    
	    virtual void reward(Player & player) = 0; //!<Method to transcribe a rewardable object to the player.
    
    };

}

#endif /* _Rewardable_H__ */
