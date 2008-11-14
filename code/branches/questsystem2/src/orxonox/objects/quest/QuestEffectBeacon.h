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

#ifndef _QuestEffectBeacon_H__
#define _QuestEffectBeacon_H__

#include "OrxonoxPrereqs.h"

#include "orxonox/objects/worldentities/PositionableEntity.h"

namespace QuestEffectBeaconStatus
{

    enum Enum
    {
        inactive,
        active
    };
    
}

namespace orxonox {

    /**
    @brief
        A QuestEffectBeacon is an entity which can (under some condition(s)) invoke a number QuestEffects on players meeting the condition(s).
        The conditions under which the QuestEffects are invoked on the player are defined by Triggers.
        A QuestEffectBeacon can be executed a defined number of times.
    @author
        Damian 'Mozork' Frick
    */
    class _OrxonoxExport QuestEffectBeacon : public PositionableEntity
    {
	public:
	    QuestEffectBeacon(BaseObject* creator);
	    virtual ~QuestEffectBeacon();
	    
	    virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
	    
	    virtual void processEvent(Event& event);
	    
	    bool execute(bool b, ControllableEntity* player); //!< Executes the QuestEffects of the QuestEffectBeacon.
	    
	    bool isActive(void); //!< Test whether the QuestEffectBeacon is active.
	    
	protected:
            bool decrementTimes(void); //!< Decrement the number of times the QuestEffectBeacon can still be executed.
            
            inline const int & getTimes(void) const //!< Return the number of times the QUestEffectBeacon can still be executed.
                { return this->times_; }

        private:
            std::list<QuestEffect*> effects_; //!< The list of QuestEffects to be invoked on the executing player.
            int times_; //!< Number of times the beacon can be exectued.
            QuestEffectBeaconStatus::Enum status_; //!< The status of the QUestEffectBeacon, Can be eighter active or inactive.
            PlayerTrigger* trigger_;
            
            bool setTimes(const int & n); //!< Set the number of times the QuestEffectBeacon can be executed.
            bool addEffect(QuestEffect* effect);
            bool addTrigger(PlayerTrigger* trigger);
            
            const QuestEffect* getEffect(unsigned int index) const;
            const PlayerTrigger* getTrigger(unsigned int index) const;
    
    };

}

#endif /* _QuestEffectBeacon_H__ */
