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
    @file QuestEffectBeacon.cc
    @brief Implementation of the QuestEffectBeacon class.
*/

#include "QuestEffectBeacon.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "core/EventIncludes.h"

#include "interfaces/PlayerTrigger.h"
#include "worldentities/pawns/Pawn.h"

#include "objects/triggers/MultiTriggerContainer.h"
#include "QuestEffect.h"

namespace orxonox
{
    CreateFactory(QuestEffectBeacon);

    /**
    @brief
        Constructor. Registers the object and initializes defaults.
    */
    QuestEffectBeacon::QuestEffectBeacon(BaseObject* creator) : StaticEntity(creator)
    {
        RegisterObject(QuestEffectBeacon);

        this->status_ = QuestEffectBeaconStatus::Active;
        this->times_ = INFINITE_TIME;
    }

    /**
        Destructor.
    */
    QuestEffectBeacon::~QuestEffectBeacon()
    {
    }

    /**
    @brief
        Method for creating a QuestEffectBeacon object through XML.
    */
    void QuestEffectBeacon::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(QuestEffectBeacon, XMLPort, xmlelement, mode);

        XMLPortParam(QuestEffectBeacon, "times", setTimes, getTimes, xmlelement, mode);
        XMLPortObject(QuestEffectBeacon, QuestEffect, "effects", addEffect, getEffect, xmlelement, mode);

        XMLPortEventSink(QuestEffectBeacon, BaseObject, "execute", execute, xmlelement, mode); //TODO: Change BaseObject to MultiTrigger as soon as MultiTrigger is the base of all triggers.

        COUT(4) << "New QuestEffectBeacon created." << std::endl;
    }

    void QuestEffectBeacon::XMLEventPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(QuestEffectBeacon, XMLEventPort, xmlelement, mode);

        XMLPortEventSink(QuestEffectBeacon, BaseObject, "execute", execute, xmlelement, mode);
    }

    /**
    @brief
        Executes the QuestEffectBeacon.
        This means extracting the Pawn from the PlayerTrigger, provided by the Event causing the execution, and the extracting the PlayerInfo from the received Pawn and invoking the QuestEffectbeacon's QuestEffects on the received PlayerInfo.
    @param bTriggered
        true means the trigger was activated while false means it was deactivated
    @param trigger
        A pointer to the PlayerTrigger that threw the Event.
    @return
        Returns true if successfully executed, false if not.
    */
    //TODO: Eliminate MultiTriggerContainer stuff, since they are now PlayerTriggers as well.
    bool QuestEffectBeacon::execute(bool bTriggered, BaseObject* trigger)
    {
        if(!bTriggered)
        {
            return false;
        }
        if(!(this->isActive())) // If the QuestEffectBeacon is inactive it cannot be executed.
        {
            COUT(4) << "The QuestEffectBeacon is inactive." << std::endl;
            return false;
        }

        PlayerTrigger* pTrigger = orxonox_cast<PlayerTrigger*>(trigger);
        MultiTriggerContainer* mTrigger = orxonox_cast<MultiTriggerContainer*>(trigger);
        Pawn* pawn = NULL;

        // If the trigger is neither a Playertrigger nor a MultiTrigger (i.e. a MultitriggerContainer) we can do anything with it.
        if(pTrigger == NULL && mTrigger == NULL)
            return false;

        // If the trigger is a PlayerTrigger.
        if(pTrigger != NULL)
        {
            if(!pTrigger->isForPlayer())  // The PlayerTrigger is not exclusively for Pawns which means we cannot extract one.
                return false;
            else
                pawn = pTrigger->getTriggeringPlayer();
        }

        // If the trigger is a MultiTrigger (i.e. a MultiTriggerContainer)
        if(mTrigger != NULL)
        {
            pawn = orxonox_cast<Pawn*>(mTrigger->getData());
        }

        if(pawn == NULL)
        {
            COUT(4) << "The QuestEffectBeacon was triggered by an entity other than a Pawn. (" << trigger->getIdentifier()->getName() << ")" << std::endl;
            return false;
        }

        // Extract the PlayerInfo from the Pawn.
        PlayerInfo* player = pawn->getPlayer();

        if(player == NULL)
        {
            COUT(3) << "The PlayerInfo* is NULL." << std::endl;
            return false;
        }

        COUT(4) << "QuestEffectBeacon executed on player: " << player << " ." << std::endl;

        bool check = QuestEffect::invokeEffects(player, this->effects_); // Invoke the QuestEffects on the PlayerInfo.
        if(check)
        {
            this->decrementTimes(); // Decrement the number of times the beacon can be used.
            return true;
        }

        return false;
    }

    /**
    @brief
        Set the status of the QuestEffectBeacon.
    @param activate
        If true the QuestEffectBeacon is activated, if false it is deactivated.
    @return
        Returns whether the activation/deactivation was successful.
    */
    bool QuestEffectBeacon::setActive(bool activate)
    {
        if(this->getTimes() == 0 && activate) // A QuestEffectBeacon that can be executed only 0 times is always inactive.
            return false;

        if(activate)
        {
            this->status_ = QuestEffectBeaconStatus::Active;
            return true;
        }

        this->status_ = QuestEffectBeaconStatus::Inactive;
        return true;
    }

    /**
    @brief
        Decrement the number of times the QuestEffectBeacon can be executed.
    @return
        Returns true if successful.
    */
    bool QuestEffectBeacon::decrementTimes(void)
    {
        if(!(this->isActive())) // The QuestEffectBeacon mus be active to decrement the number of times it can be executed.
            return false;

        if(this->getTimes() == INFINITE_TIME) // If times is infinity the QuestEffectBeacon can be executed an infinite number fo times.
            return true;

        this->times_ = this->times_ - 1; // Decrement number of times the QuestEffectBeacon can be executed.
        if(this->getTimes() == 0) // Set the QuestEffectBeacon to inactive when the number of times it can be executed is reduced to 0.
            this->status_ = QuestEffectBeaconStatus::Inactive;

        return true;
    }

    /**
    @brief
        Set the number of times the QuestEffectBeacon can be executed.
        The number must be eighter <= 0, or INFINITY which is '-1'.
    @param n
        The number of times the QuestEffectBeacon can be executed.
        The number must be eighter <= 0, or INFINITY which is '-1'.
    @return
        Returns true if successful.
    */
    bool QuestEffectBeacon::setTimes(const int & n)
    {
        if(n < 0 && n != INFINITE_TIME)
            return false;

        this->times_ = n;
        return true;
    }

    /**
    @brief
        Adds a QuestEffect to the QuestEffectBeacon.
    @param effect
        A pointer to the QuestEffect to be added.
    @return
        Returns true if successful.
    */
    bool QuestEffectBeacon::addEffect(QuestEffect* effect)
    {
        //TODO: Replace with assert.
        if(effect == NULL) // NULL-pointers are not well liked here...
        {
            COUT(2) << "A NULL-QuestEffect was trying to be added" << std::endl;
            return false;
        }

        this->effects_.push_back(effect);

        COUT(4) << "A QuestEffect was added to a QuestEffectBeacon." << std::endl;
        return true;
    }

    /**
    @brief
        Returns the QuestEffect at the given index.
    @param index
        The index.
    @return
        Returns a pointer to the QuestEffect at the given index.
    */
    const QuestEffect* QuestEffectBeacon::getEffect(unsigned int index) const
    {
        int i = index;
        for (std::list<QuestEffect*>::const_iterator effect = this->effects_.begin(); effect != this->effects_.end(); ++effect)
        {
            if(i == 0)
               return *effect;

            i--;
        }
        return NULL;
    }

}