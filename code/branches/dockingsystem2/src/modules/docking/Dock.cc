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
 *      Sven Stucki
 *   Co-authors:
 *      ...
 *
 */

/**
    @file Dock.cc
	@brief Docking system main class
*/

#include "Dock.h"
#include "infos/HumanPlayer.h"
#include "worldentities/pawns/Pawn.h"
#include "interfaces/PlayerTrigger.h"


namespace orxonox
{
    CreateFactory(Dock);

    Dock::Dock(BaseObject* creator) : StaticEntity(creator)
    {
        RegisterObject(Dock);
        COUT(0) << "Registering dock..." << std::endl;
    }

    Dock::~Dock()
    {
    }


    void Dock::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(Dock, XMLPort, xmlelement, mode);

        XMLPortObject(Dock, DockingEffect, "effects", addEffect, getEffect, xmlelement, mode);
        XMLPortEventSink(Dock, BaseObject, "execute", execute, xmlelement, mode);

        COUT(0) << "Dock created.." << std::endl;
    }

    void Dock::XMLEventPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(Dock, XMLEventPort, xmlelement, mode);

        XMLPortEventSink(Dock, BaseObject, "execute", execute, xmlelement, mode);
    }


    bool Dock::execute(bool bTriggered, BaseObject* trigger)
    {
        //TODO: Handle DistanceMultiTrigger

        PlayerTrigger* pTrigger = orxonox_cast<PlayerTrigger*>(trigger);
        Pawn* pawn = NULL;

        // Check whether it is a player trigger and extract pawn from it
        if(pTrigger != NULL)
        {
            if(!pTrigger->isForPlayer()) {  // The PlayerTrigger is not exclusively for Pawns which means we cannot extract one.
                COUT(0) << "Docking:execute PlayerTrigger was not triggered by a player.." << std::endl;
                return false;
            }
            pawn = pTrigger->getTriggeringPlayer();
        } else {
            COUT(0) << "Docking::execute Not a player trigger, can't extract pawn from it.." << std::endl;
            return false;
        }
        if(pawn == NULL)
        {
            COUT(0) << "Docking::execute Can't retrieve Pawn from Trigger. (" << trigger->getIdentifier()->getName() << ")" << std::endl;
            return false;
        }

        // Extract the PlayerInfo from the Pawn.
        PlayerInfo* player = pawn->getPlayer();
        if(player == NULL)
        {
            COUT(0) << "The PlayerInfo* is NULL." << std::endl;
            return false;
        }

        // Try to get HumanPlayer
        if(!player->isHumanPlayer()) {
            COUT(0) << "Docking::execute Not triggered by a human." << std::endl;
            return false;
        }
        HumanPlayer* human = orxonox_cast<HumanPlayer*>(player);
        if(human == NULL) {
            COUT(0) << "Docking::execute Player was not as human as expected.." << std::endl;
        }
        COUT(0) << "Dock triggered by player: " << human->getName() << ".." << std::endl;

        //TODO: This is waaay too oversimplified
        if(bTriggered) {
            DockingEffect::invokeEffect(docking::DOCKING, player, effects_);
            //DockingEffect::invokeEffect(docking::ATTACH, player, effects_);
        } else {
            DockingEffect::invokeEffect(docking::RELEASE, player, effects_);
        }

        return true;
    }


    bool Dock::addEffect(DockingEffect* effect) {
        assert(effect);
        effects_.push_back(effect);
        return true;
    }

    const DockingEffect* Dock::getEffect(unsigned int index) const {
        int i = index;
        for (std::list<DockingEffect*>::const_iterator effect = this->effects_.begin(); effect != this->effects_.end(); ++effect)
        {
            if(i == 0)
               return *effect;

            i--;
        }
        return NULL;
    }
}
