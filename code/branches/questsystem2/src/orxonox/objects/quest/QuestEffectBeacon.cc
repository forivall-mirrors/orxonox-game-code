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

#include "OrxonoxStableHeaders.h"
#include "QuestEffectBeacon.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "core/Event.h"
#include "core/EventIncludes.h"

#include "orxonox/objects/infos/PlayerInfo.h"
#include "orxonox/objects/worldentities/ControllableEntity.h"
#include "orxonox/objects/worldentities/triggers/PlayerTrigger.h"
#include "QuestEffect.h"

namespace orxonox {

    CreateFactory(QuestEffectBeacon);

    QuestEffectBeacon::QuestEffectBeacon(BaseObject* creator) : PositionableEntity(creator)
    {
        RegisterObject(QuestEffectBeacon);
        
        this->status_ = QuestEffectBeaconStatus::active;
        this->times_ = -1;
        this->trigger_ = NULL;
    }

    QuestEffectBeacon::~QuestEffectBeacon()
    {
    }
    
    void QuestEffectBeacon::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(QuestEffectBeacon, XMLPort, xmlelement, mode);

        XMLPortParam(QuestEffectBeacon, "times", setTimes, getTimes, xmlelement, mode);
        XMLPortObject(QuestEffectBeacon, QuestEffect, "", addEffect, getEffect, xmlelement, mode);
        XMLPortObject(QuestEffectBeacon, PlayerTrigger, "", addTrigger, getTrigger, xmlelement, mode);
    }
    
    void QuestEffectBeacon::processEvent(Event& event)
    {
	SetSubclassEvent(QuestEffectBeacon, "execute", execute, event, PlayerTrigger);
    }
    
    bool QuestEffectBeacon::execute(bool b, ControllableEntity* entity)
    {
        if(!b || !(this->isActive()))
        {
            return false;
        }
        if(entity == NULL)
        {
            return false;
        }
        
        PlayerInfo* player = entity->getPlayer();
        
        bool check = QuestEffect::invokeEffects(player, this->effects_);
        if(check)
        {
            this->decrementTimes();
            return true;
	}
	
	return false;
    }
    
    bool QuestEffectBeacon::isActive(void)
    {
        return this->status_ == QuestEffectBeaconStatus::active;
    }
    
    bool QuestEffectBeacon::decrementTimes(void)
    {
        if(!(this->isActive()))
        {
            return false;
        }
        if(this->getTimes() == -1)
        {
            return true;
        }
        
        this->times_ = this->times_ - 1;
	if(this->getTimes() == 0)
	{
            this->status_ = QuestEffectBeaconStatus::inactive;
	}
        
        return true;
    }
    
    
    bool QuestEffectBeacon::setTimes(const int & n)
    {
        if(n < -1)
        {
            return false;
        }
        
        this->times_ = n;
        return true;
    }
    
    
    /**
    @brief

    */
    bool QuestEffectBeacon::addEffect(QuestEffect* effect)
    {
        if(effect == NULL)
        {
            COUT(2) << "A NULL-QuestEffect was trying to be added" << std::endl;
            return false;
        }

        this->effects_.push_back(effect);

        COUT(3) << "A QuestEffect was added to a QuestEffectBeacon." << std::endl;
        return true;
    }
    
    bool QuestEffectBeacon::addTrigger(PlayerTrigger* trigger)
    {
        if(this->trigger_ != NULL)
	{
	   COUT(2) << "A Trigger was trying to be added, where one was already set." << std::endl;
	   return false;
	}
	if(trigger == NULL)
	{
            COUT(2) << "A NULL-Trigger was trying to be added." << std::endl;
            return false;
	}
	
	COUT(3) << "A Trigger was added to a QuestEffectBeacon." << std::endl;
	this->trigger_ = trigger;
	return true;
    }
    
     /**
    @brief

    */
    const QuestEffect* QuestEffectBeacon::getEffect(unsigned int index) const
    {
        int i = index;
        for (std::list<QuestEffect*>::const_iterator effect = this->effects_.begin(); effect != this->effects_.end(); ++effect)
        {
            if(i == 0)
            {
               return *effect;
            }
            i--;
        }
        return NULL;
    }

    const PlayerTrigger* QuestEffectBeacon::getTrigger(unsigned int index) const
    {
        if(index == 0)
        {
            return this->trigger_;
        }
        
        return NULL;
    }

}
