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
    @file Pickup.cc
    @brief Implementation of the Pickup class.
*/

#include "Pickup.h"

#include "core/CoreIncludes.h"
#include "util/StringUtils.h"

#include "DroppedPickup.h"

namespace orxonox
{

    //! Initializing the static strings.
    /*static*/ const std::string Pickup::activationTypeImmediate_s = "immediate";
    /*static*/ const std::string Pickup::activationTypeOnUse_s = "onUse";
    /*static*/ const std::string Pickup::durationTypeOnce_s = "once";
    /*static*/ const std::string Pickup::durationTypeContinuous_s = "continuous";

    CreateUnloadableFactory(Pickup);

    /**
    @brief
        Constructor. Registers and initializes the object.
    @param creator
        The objects creator.
    */
    Pickup::Pickup(BaseObject* creator) : BaseObject(creator)
    {
        RegisterObject(Pickup);

        this->initialize();
    }

    /**
    @brief
        Destructor.
    */
    Pickup::~Pickup()
    {

    }

    /**
    @brief
        Initializes the member variables.
    */
    void Pickup::initialize(void)
    {
        this->activationType_ = pickupActivationType::immediate;
        this->durationType_ = pickupDurationType::once;
    }

    /**
    @brief
        Method for creating a Pickup object through XML.
    */
    void Pickup::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(Pickup, XMLPort, xmlelement, mode);

        XMLPortParam(Pickup, "representation", setRepresentationName, getRepresentationName, xmlelement, mode);
        XMLPortParam(Pickup, "activationType", setActivationTypeAsString, getActivationTypeAsString, xmlelement, mode);
        XMLPortParam(Pickup, "durationType", setDurationTypeAsString, getDurationTypeAsString, xmlelement, mode);
    }

    /**
    @brief
        Get the activation type of the pickup.
    @return
        Returns a string containing the activation type.
    */
    const std::string& Pickup::getActivationTypeAsString(void) const
    {
        switch(this->getActivationType())
        {
            case pickupActivationType::immediate:
                return activationTypeImmediate_s;
            case pickupActivationType::onUse:
                return activationTypeOnUse_s;
            default:
                return BLANKSTRING;
        }
    }

    /**
    @brief
        Get the duration type of the pickup.
    @return
        Returns a string containing the duration type.
    */
    const std::string& Pickup::getDurationTypeAsString(void) const
    {
        switch(this->getDurationType())
        {
            case pickupDurationType::once:
                return durationTypeOnce_s;
            case pickupDurationType::continuous:
                return durationTypeContinuous_s;
            default:
                return BLANKSTRING;
        }
    }

    /**
    @brief
        Set the activation type of the Pickup.
    @param type
        The activation type of the Pickup as a string.
    */
    void Pickup::setActivationTypeAsString(const std::string& type)
    {
        if(type == Pickup::activationTypeImmediate_s)
            this->setActivationType(pickupActivationType::immediate);
        else if(type == Pickup::activationTypeOnUse_s)
            this->setActivationType(pickupActivationType::onUse);
        else
            orxout(internal_error, context::pickups) << "Invalid activationType '" << type << "' in " << this->getIdentifier()->getName() << "." << endl;
    }

    /**
    @brief
        Set the duration type of the Pickup.
    @param type
        The duration type of the Pickup as a string.
    */
    void Pickup::setDurationTypeAsString(const std::string& type)
    {
        if(type == Pickup::durationTypeOnce_s)
            this->setDurationType(pickupDurationType::once);
        else if(type == Pickup::durationTypeContinuous_s)
            this->setDurationType(pickupDurationType::continuous);
        else
            orxout(internal_error, context::pickups) << "Invalid durationType '" << type << "' in " << this->getIdentifier()->getName() << "." << endl;
    }

    /**
    @brief
        Should be called when the pickup has transited from picked up to dropped or the other way around.
        Any Class overwriting this method must call its SUPER function by adding SUPER(Classname, changedPickedUp); to their changedPickedUp method.
    */
    void Pickup::changedPickedUp(void)
    {
        SUPER(Pickup, changedPickedUp);

        // Sets the Pickup to used if the Pickup has activation type 'immediate' and gets picked up.
        if(this->isPickedUp() && this->isImmediate())
            this->setUsed(true);
    }

    /**
    @brief
        Creates a duplicate of the OrxonoxClass.
    @param item
        A reference to the pointer of the item that we're duplicating.
    */
    void Pickup::clone(OrxonoxClass*& item)
    {
        if(item == NULL)
            item = new Pickup(this);

        SUPER(Pickup, clone, item);

        Pickup* pickup = orxonox_cast<Pickup*>(item);
        pickup->setRepresentationName(this->getRepresentationName());
        pickup->setActivationType(this->getActivationType());
        pickup->setDurationType(this->getDurationType());
    }

    /**
    @brief
        Facilitates the creation of a PickupSpawner upon dropping of the Pickupable.
        This method must be implemented by any class directly inheriting from Pickupable. It is most easily done by just creating a new DroppedPickup, e.g.:
        DroppedPickup(BaseObject* creator, Pickupable* pickup, const Vector3& position);
    @return
        Returns true if a spawner was created, false if not.
    */
    bool Pickup::createSpawner(void)
    {
        new DroppedPickup(this, this, this->getCarrier());
        return true;
    }

}
