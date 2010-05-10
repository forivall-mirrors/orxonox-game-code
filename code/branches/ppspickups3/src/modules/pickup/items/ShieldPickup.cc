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
 *      Eric Beier
 *   Co-authors:
 *      ...
 *
 */

/**
    @file ShieldPickup.cc
    @brief Implementation of the ShieldPickup class.
*/

#include "ShieldPickup.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "util/StringUtils.h"

#include "worldentities/pawns/SpaceShip.h"
#include "items/Engine.h"
#include "pickup/PickupIdentifier.h"

#include <sstream>


namespace orxonox
{
    CreateFactory(ShieldPickup);

    /**
    @brief
        Constructor. Registers the object and initializes the member variables.
    */
    ShieldPickup::ShieldPickup(BaseObject* creator) : Pickup(creator)
    {
        RegisterObject(ShieldPickup);

        this->initialize();
    }

    /**
    @brief
        Destructor.
    */
    ShieldPickup::~ShieldPickup()
    {

    }

    /**
    @brief
        Initializes the member variables.
    */
    void ShieldPickup::initialize(void)
    {
        this->duration_ = 0.0f;

        this->addTarget(ClassIdentifier<Engine>::getIdentifier());
    }

    /**
    @brief
        Initializes the PickupIdentifier of this pickup.
    */
    void ShieldPickup::initializeIdentifier(void)
    {
        std::stringstream stream;
        stream << this->getDuration();
        std::string type1 = "duration";
        std::string val1 = stream.str();
        this->pickupIdentifier_->addParameter(type1, val1);

//         stream.clear();
//         stream << this->getShieldAdd();
//         std::string type2 = "ShieldAdd";
//         std::string val2 = stream.str();
//         this->pickupIdentifier_->addParameter(type2, val2);

    }

    /**
    @brief
        Method for creating a ShieldPickup object through XML.
    */
    void ShieldPickup::XMLPort(Element& xmlelement, orxonox::XMLPort::Mode mode)
    {
        SUPER(ShieldPickup, XMLPort, xmlelement, mode);

        XMLPortParam(ShieldPickup, "duration", setDuration, getDuration, xmlelement, mode);

        this->initializeIdentifier();
    }

    /**
    @brief
        Is called when the pickup has transited from used to unused or the other way around.
    */
    void ShieldPickup::changedUsed(void)
    {
        SUPER(ShieldPickup, changedUsed);

        //! If the pickup is not picked up nothing must be done.
        if(!this->isPickedUp())
            return;
    }

    /**
    @brief
        Creates a duplicate of the input OrxonoxClass.
    @param item
        A pointer to the Orxonox class.
    */
    void ShieldPickup::clone(OrxonoxClass*& item)
    {
        if(item == NULL)
            item = new ShieldPickup(this);

        SUPER(ShieldPickup, clone, item);

        ShieldPickup* pickup = dynamic_cast<ShieldPickup*>(item);
        pickup->setDuration(this->getDuration());

        pickup->initializeIdentifier();
    }

    /**
    @brief
        Sets the duration.
    @param duration
        The duration
    */
    void ShieldPickup::setDuration(float duration)
    {
        if(duration >= 0.0f)
        {
            this->duration_ = duration;
        }
        else
        {
            COUT(1) << "Invalid duration in ShieldPickup." << std::endl;
            this->duration_ = 0.0f;
        }
    }

    void ShieldPickup::pickupTimerCallback(void)
    {       
        this->setUsed(false);
    }
}
