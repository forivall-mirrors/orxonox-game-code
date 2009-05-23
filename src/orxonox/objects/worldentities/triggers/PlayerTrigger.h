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
    @brief
    Definition of the PlayerTrigger class.
*/

#ifndef _PlayerTrigger_H__
#define _PlayerTrigger_H__

#include "OrxonoxPrereqs.h"

#include "Trigger.h"

namespace orxonox
{
    /**
    @brief
        A PlayerTrigger is a trigger which is normally triggered by ControllableEntities and can as such return a pointer to the ControllableEntity which triggered it.
    @author
        Damian 'Mozork' Frick
    */
    class _OrxonoxExport PlayerTrigger : public Trigger
    {
    public:
        PlayerTrigger(BaseObject* creator);
        virtual ~PlayerTrigger();
        
        virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode); //!< Method for creating a PlayerTrigger object through XML.
        
        /**
        @brief Returns the player that triggered the PlayerTrigger.
        @return Returns a pointer to the ControllableEntity that triggered the PlayerTrigger.
        */
        inline Pawn* getTriggeringPlayer(void) const
            { return this->player_; }
        
        /**
        @brief Checks whether the PlayerTrigger normally returns a ControllableEntity.
        @return Returns true if the PlayerTrigger normally returns a ControllableEntity.
        */
        inline bool isForPlayer(void) const
           { return this->isForPlayer_; }
            
    protected:
        virtual bool isTriggered(TriggerMode mode) = 0;
        
        /**
        @brief Set the player that triggered the PlayerTrigger. This is normally done by classes inheriting vom PlayerTrigger.
        @param player A pointer to the ControllableEntity that triggered the PlayerTrigger.
        */
        inline void setTriggeringPlayer(Pawn* player)
           { this->player_ = player; }

            /**
            @brief Set whether the PlayerTrigger normally is triggered by ControllableEntities.
            @param isForPlayer Should be true when the PlayerTrigger schould be set to normally be triggered by ControllableEntities, false if not.
            */
        inline void setForPlayer(bool isForPlayer)
           { this->isForPlayer_ = isForPlayer; }
        
    private:
        Pawn* player_; //!< The player that triggered the PlayerTrigger.
        bool isForPlayer_; //!< Is true when the PlayerTrigger schould be set to normally be triggered by ControllableEntities.
    
    };

}

#endif /* _PlayerTrigger_H__ */
