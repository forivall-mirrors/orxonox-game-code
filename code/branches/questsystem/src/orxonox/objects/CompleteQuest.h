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

#ifndef _CompleteQuest_H__
#define _CompleteQuest_H__

#include <string>

#include "ChangeQuestStatus.h"

namespace orxonox {

    class Player; //Forward declaration, remove when fully integrated into the objecthirarchy.

    /**
    @brief
        Completes a quest.
    @author
        Damian 'Mozork' Frick
    */
    class CompleteQuest : public ChangeQuestStatus
    {
	public:
            CompleteQuest();
	    CompleteQuest(std::string questId);
	    ~CompleteQuest();
	    
	    virtual void invoke(Player* player); //!< Invokes the effect.
    
    };

}

#endif /* _CompleteQuest_H__ */
