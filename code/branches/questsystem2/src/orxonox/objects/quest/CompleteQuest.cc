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
    @file CompleteQuest.cc
    @brief
	Implementation of the CompleteQuest class.
*/

#include "OrxonoxStableHeaders.h"
#include "CompleteQuest.h"

#include "core/CoreIncludes.h"
#include "util/Exception.h"

#include "orxonox/objects/worldentities/ControllableEntity.h"
#include "QuestManager.h"
#include "Quest.h"

namespace orxonox {

    CreateFactory(CompleteQuest);

    /**
    @brief
        Constructor. Registers the object.
    */
    CompleteQuest::CompleteQuest(BaseObject* creator) : ChangeQuestStatus(creator)
    {
        RegisterObject(CompleteQuest);
    }

    /**
    @brief
        Destructor.
    */
    CompleteQuest::~CompleteQuest()
    {
    }

    /**
    @brief
        Method for creating a CompleteQuest object through XML.
    */
    void CompleteQuest::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(CompleteQuest, XMLPort, xmlelement, mode);
    }

    /**
    @brief
        Invokes the effect.
    @param player
        The player the effect is invoked on.
    @return
        Returns true if the effect was invoked successfully.
    */
    bool CompleteQuest::invoke(ControllableEntity* player)
    {
        if(player == NULL) //!< You know, what we think of NULL-pointers...
        {
            COUT(2) << "Input player is NULL." << std::endl;
            return false;
        }

        try
        {
            Quest* quest = QuestManager::findQuest(this->getQuestId());
            if(!quest->complete(player))
            {
               return false;
            }
        }
        catch(const Exception& e)
        {
            COUT(2) << e.getFullDescription() << std::endl;
            return false;
        }

        return true;
    }


}
