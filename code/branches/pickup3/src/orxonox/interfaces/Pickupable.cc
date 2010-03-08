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
    @brief Implementation of the Pickupable class.
*/

#include "Pickupable.h"

#include "core/Identifier.h"
#include "core/CoreIncludes.h"
#include "pickup/PickupIdentifier.h"
#include "PickupCarrier.h"

namespace orxonox
{
    
    /**
    @brief
        Constructor. Registers the objects and initializes its member variables.
    */
    Pickupable::Pickupable()
    {
        this->used_ = false;
        this->pickedUp_ = false;
        
        RegisterRootObject(Pickupable);
        
        this->carrier_ = NULL;
        
        this->pickupIdentifier_ = new PickupIdentifier(this);
    }
    
    /**
    @brief
        Destructor. 
    */
    Pickupable::~Pickupable()
    {
        if(this->isUsed())
            this->setUsed(false);
        
        if(this->isPickedUp() && this->getCarrier() != NULL)
        {
            this->getCarrier()->drop(this, false);
            this->setCarrier(NULL);
        }
    }
    
    /**
    @brief
        Sets the Pickupable to used or unused, depending on the input.
    @param used
        If used is true the Pickupable is set to used, it is set to unused, otherwise.
    @return
        Returns true if the used state was changed, false if not.
    */
    bool Pickupable::setUsed(bool used)
    {
        if(this->used_ == used)
            return false;
        
        COUT(4) << "Pickupable (&" << this << ") set to used " << used << "." << std::endl;
        
        this->used_ = used;
        this->changedUsed();
        return true;
    }
    
    /**
    @brief
        Get whether the given PickupCarrier is a target of this pickup.
    @param carrier
        The PickupCarrier of which it has to be determinde whether it is a target of this pickup.
    @return
        Returns true if the given PickupCarrier is a target.
    */
    bool Pickupable::isTarget(const PickupCarrier* carrier) const
    {
        return this->isTarget(carrier->getIdentifier());
    }
    
    /**
    @brief
        Get whether a given class, represented by the input Identifier, is a target of this pickup.
    @param target
        The Identifier of which it has to be determinde whether it is a target of this pickup.
    @return
        Returns true if the given Identifier is a target.
    */
    bool Pickupable::isTarget(Identifier* target) const
    {
        //! Iterate through all targets of this Pickupable.
        for(std::list<Identifier*>::const_iterator it = this->targets_.begin(); it != this->targets_.end(); it++)
        {
            if(target->isA(*it))
                return true;
        }
        return false;
    }
        
    /**
    @brief
        Add a PickupCarrier as target of this pickup.
    @param target
        The PickupCarrier to be added.
    @return
        Returns true if the target was added, false if not.
    */
    bool Pickupable::addTarget(PickupCarrier* target)
    {
        return this->addTarget(target->getIdentifier());
    }
    
    /**
    @brief
        Add a class, representetd by the input Identifier, as target of this pickup.
    @param target
        The Identifier to be added.
    @return
        Returns true if the target was added, false if not.
    */
    bool Pickupable::addTarget(Identifier* target)
    {
        if(this->isTarget(target)) //!< If the input target is already present in the list of targets.
            return false;
        
        COUT(4) << "Target " << target->getName() << " added to Pickupable (&" << this << ")." << std::endl;
        this->targets_.push_back(target);
        return true;
    }
    
    /**
    @brief  
        Sets the Pickupable to picked up.
        This method will be called by the PickupCarrier picking the Pickupable up.
    @param carrier
        The PickupCarrier that picked the Pickupable up.
    @return
        Returns false if, for some reason, the pickup could not be picked up, e.g. it was picked up already.
    */
    bool Pickupable::pickedUp(PickupCarrier* carrier)
    {
        if(this->isPickedUp()) //!< If the Pickupable is already picked up.
            return false;
        
        COUT(4) << "Pickupable (&" << this << ") got picked up by a PickupCarrier (&" << carrier << ")." << std::endl;
        this->setPickedUp(true);
        this->setCarrier(carrier);
        return true;
    }
    
    /**
    @brief 
        Sets the Pickupable to not picked up or dropped.
        This method will be called by the PickupCarrier dropping the Pickupable.
    @return
        Returns false if the pickup could not be dropped.
    */
    bool Pickupable::dropped(void)
    {
        if(!this->isPickedUp()) //!< If the Pickupable is not picked up.
            return false;
        
        COUT(4) << "Pickupable (&" << this << ") got dropped up by a PickupCarrier (&" << this->getCarrier() << ")." << std::endl;
        this->setUsed(false);
        this->setPickedUp(false);
        
        bool created = this->createSpawner(this->getCarrier()->getCarrierPosition());
        
        this->setCarrier(NULL);
        //TODO: possible problem.
        if(!created)
            this->destroy();
        
        return true;
    }
    
        
    /**
    @brief
        Sets the carrier of the pickup.
    @param carrier
        Sets the input PickupCarrier as the carrier of the pickup.
    @return
        Returns true if the carrier was changed, false if not.
    */
    bool Pickupable::setCarrier(PickupCarrier* carrier)
    {
        if(this->getCarrier() == carrier)
            return false;
        
        this->carrier_ = carrier;
        this->changedCarrier();
        return true;
    }
    
    /**
    @brief
        Creates a duplicate of the Pickupable.
    @return
        Returns the clone of this pickup as a pointer to a Pickupable.
    */
    //TODO: Does this work?
    Pickupable* Pickupable::clone(void)
    {
        OrxonoxClass* item = NULL;
        this->clone(item);
        
        Pickupable* pickup = dynamic_cast<Pickupable*>(item);
        
        COUT(4) << "Pickupable (&" << this << ") cloned. Clone is new Pickupable (&" << pickup << ")." << std::endl;
        return pickup;
    }
    
    /**
    @brief
        Creates a duplicate of the input OrxonoxClass.
        This method needs to be implemented by any Class inheriting from Pickupable.
    @param item
        A pointer to the OrxonoxClass that is to be duplicated.
    */
    //TODO: Specify how the implementation must be done in detail.
    void Pickupable::clone(OrxonoxClass*& item)
    {
        SUPER(Pickupable, clone, item);
    }
    
}
