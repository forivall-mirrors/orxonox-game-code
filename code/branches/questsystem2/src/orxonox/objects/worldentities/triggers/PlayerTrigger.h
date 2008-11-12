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
 
#ifndef _PlayerTrigger_H__
#define _PlayerTrigger_H__

#include "Trigger.h"

namespace orxonox {
    
    /**
    @brief
        
    @author
        Damian 'Mozork' Frick
    */
    class _OrxonoxExport PlayerTrigger : public Trigger
    {
	public:
	    PlayerTrigger(BaseObject* creator);
	    virtual ~PlayerTrigger();
	    
            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            
            inline ControllableEntity* getTriggeringPlayer(void) const
                { return this->player_; }
            
	protected:
	    virtual bool isTriggered(TriggerMode mode) = 0;
	    
	    inline void setTriggeringPlayer(ControllableEntity* player)
	       { this->player_ = player; }
	    inline bool isForPlayer(void) const
	       { return this->isForPlayer_; }
	    inline void setForPlayer(bool isForPlayer)
	       { this->isForPlayer_ = isForPlayer; }
	    
	private:
	    ControllableEntity* player_;
	    bool isForPlayer_;
	
    };

}

#endif /* _PlayerTrigger_H__ */
