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

#include "core/CoreIncludes.h"
#include "QuestHint.h"

namespace orxonox {

    CreateFactory(QuestHint);

    /**
    @brief
        Constructor.  Needs as input a unique identifier to be able to identify different instances of this class (and subclasses).
    @param id
        The unique identifier.
    @param title
        The title of the hint.
    @param description
        The description of the hint, resp. the hint itself.
    */
    QuestHint::QuestHint(std::string id, std::string title = "", std::string description = "") : QuestItem(id, title, description)
    {
        RegisterObject(QuestHint);
    }
    
    /**
    @brief
        Destructor.
    */
    QuestHint::~QuestHint()
    {
        
    }
    
    bool QuestHint::isActive(const Player & player) const
    {
        //TDO: Implement.
    }
    
    void QuestHint::activate(const Player & player)
    {
        if(this->quest_->isActive(player))
        {
            //TDO: Implement.
        }
        else
        {
            COUT(2) << "A hint of a non-active quest was trying to get activated." << std::endl;
        }
    }

    void QuestHint::setQuest(Quest* quest)
    {
        this->quest_ = quest;
    }

}
