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
#include "util/Exception.h"

#include "Quest.h"
#include "QuestHint.h"

namespace orxonox {

    CreateFactory(QuestHint);

    /**
    @brief
        Constructor.
    */
    QuestHint::QuestHint() : QuestItem()
    {
        this->initialize();
    }

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
    QuestHint::QuestHint(std::string id) : QuestItem(id)
    {
        this->initialize();
    }
    
    /**
    @brief
        Destructor.
    */
    QuestHint::~QuestHint()
    {
        
    }
    
    void QuestHint::initialize(void)
    {
        RegisterObject(QuestHint);
    }
    
    void QuestHint::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
	SUPER(QuestHint, XMLPort, xmlelement, mode);
    }

    
    /**
    @brief
        Checks whether the hint is active for a specific player.
    @param player
        The player.
    @throws
        Throws an Argument Exception if the input Player-pointer is NULL.
    @return
        Returns true if the hint is active for the specified player.
    */
    bool QuestHint::isActive(Player* player)
    {
        if(player == NULL)
        {
            ThrowException(Argument, "The input Player* is NULL.");
            return false;
        }
        
        std::map<Player*, questHintStatus::Enum>::iterator it = this->playerStatus_.find(player);
	if (it != this->playerStatus_.end())
	{
	    return it->second;
	}
	return questStatus::inactive;
    }
    
    /**
    @brief
        Activates a QuestHint for a given player.
    @param player
        The player.
    @return
        Returns true if the activation was successful, false if there were problems.
    */
    bool QuestHint::activate(Player* player)
    {
        if(this->quest_->isActive(player))
        {
	    if(!(this->isActive(player)))
	    {
		this->playerStatus_[player] = questHintStatus::active;
		return true;
	    }
	    else
	    {
                COUT(2) << "An already active questHint was trying to get activated." << std::endl;
                return false;
	    }
        }
	COUT(2) << "A hint of a non-active quest was trying to get activated." << std::endl;
	return false;
    }

    /**
    @brief
        Sets the quest the QuestHitn belongs to.
    @param quest
    @return
    */
    bool QuestHint::setQuest(Quest* quest)
    {
        if(quest == NULL)
        {
            COUT(2) << "The input Quest* is NULL." << std::endl;
            return false;
        }
        
        this->quest_ = quest;
        return true;
    }

}
