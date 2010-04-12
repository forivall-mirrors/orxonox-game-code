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
    @file SpeedPickup.cc
    @brief Implementation of the SpeedPickup class.
*/

#include "SpeedPickup.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "util/StringUtils.h"

#include "worldentities/pawns/SpaceShip.h"
#include "items/Engine.h"
#include "pickup/PickupIdentifier.h"

#include <sstream>


namespace orxonox
{
    CreateFactory(SpeedPickup);

    /**
    @brief
        Constructor. Registers the object and initializes the member variables.
    */
    SpeedPickup::SpeedPickup(BaseObject* creator) : Pickup(creator)
    {
        RegisterObject(SpeedPickup);

        this->initialize();
    }

    /**
    @brief
        Destructor.
    */
    SpeedPickup::~SpeedPickup()
    {

    }

    /**
    @brief
        Initializes the member variables.
    */
    void SpeedPickup::initialize(void)
    {
        this->duration_ = 0.0;
        this->speedAdd_ = 0.0;
        this->speedMultiply_ = 1.0;

        this->addTarget(ClassIdentifier<Pawn>::getIdentifier());
    }

    /**
    @brief
        Initializes the PickupIdentifier of this pickup.
    */
    void SpeedPickup::initializeIdentifier(void)
    {
        std::stringstream stream;
        stream << this->getDuration();
        std::string type1 = "duration";
        std::string val1 = stream.str();
        this->pickupIdentifier_->addParameter(type1, val1);

        stream.clear();
        stream << this->getSpeedAdd();
        std::string type2 = "speedAdd";
        std::string val2 = stream.str();
        this->pickupIdentifier_->addParameter(type2, val2);

        stream.clear();
        stream << this->getSpeedMultiply();
        std::string type3 = "speedMultiply";
        std::string val3 = stream.str();
        this->pickupIdentifier_->addParameter(type3, val3);
    }

    /**
    @brief
        Method for creating a SpeedPickup object through XML.
    */
    void SpeedPickup::XMLPort(Element& xmlelement, orxonox::XMLPort::Mode mode)
    {
        SUPER(SpeedPickup, XMLPort, xmlelement, mode);

        XMLPortParam(SpeedPickup, "duration", setDuration, getDuration, xmlelement, mode);
        XMLPortParam(SpeedPickup, "speedAdd", setSpeedAdd, getSpeedAdd, xmlelement, mode);
        XMLPortParam(SpeedPickup, "speedMultiply", setSpeedMultiply, getSpeedMultiply, xmlelement, mode);

        this->initializeIdentifier();
    }

    /**
    @brief
        Is called when the pickup has transited from used to unused or the other way around.
    */
    void SpeedPickup::changedUsed(void)
    {
        SUPER(SpeedPickup, changedUsed);

        //! If the pickup is not picked up nothing must be done.
        if(!this->isPickedUp())
            return;

        //! If the pickup has transited to used.
        if(this->isUsed())
        {
            Engine* engine = this->carrierToEngineHelper();
            if(engine == NULL) //!< If the PickupCarrier is no Pawn, then this pickup is useless and therefore is destroyed.
                this->destroy();

            this->startPickupTimer(this->getDuration());
            engine->setSpeedAdd(this->getSpeedAdd());
            engine->setSpeedMultiply(this->getSpeedMultiply());
        }
    }



    /**
    @brief
        Helper to transform the PickupCarrier to a Pawn, and throw an error message if the conversion fails.
    @return
        A pointer to the Pawn, or NULL if the conversion failed.
    */
    Engine* SpeedPickup::carrierToEngineHelper(void)
    {
        PickupCarrier* carrier = this->getCarrier();
        SpaceShip* ship = dynamic_cast<SpaceShip*>(carrier);

        if(ship == NULL)
        {
            COUT(1) << "Invalid PickupCarrier in SpeedPickup." << std::endl;
        }
        else
        {
            return ship->getEngine();
        }

        return 0;
    }

    /**
    @brief
        Creates a duplicate of the input OrxonoxClass.
    @param item
        A pointer to the Orxonox class.
    */
    void SpeedPickup::clone(OrxonoxClass*& item)
    {
        if(item == NULL)
            item = new SpeedPickup(this);

        SUPER(SpeedPickup, clone, item);

        SpeedPickup* pickup = dynamic_cast<SpeedPickup*>(item);
        pickup->setDuration(this->getDuration());
        pickup->setSpeedAdd(this->getSpeedAdd());
        pickup->setSpeedMultiply(this->getSpeedMultiply());

        pickup->initializeIdentifier();
    }

    /**
    @brief
        Sets the duration.
    @param duration
        The duration
    */
    void SpeedPickup::setDuration(float duration)
    {
        if(duration >= 0.0f)
        {
            this->duration_ = duration;
        }
        else
        {
            COUT(1) << "Invalid duration in SpeedPickup." << std::endl;
            this->duration_ = 0.0f;
        }
    }

    /**
    @brief
        Sets the SpeedAdd
    @param speedAdd
        The added Speed
    */
    void SpeedPickup::setSpeedAdd(float speedAdd)
    {
        if(speedAdd >= 0.0f)
        {
            this->speedAdd_ = speedAdd;
        }
        else
        {
            COUT(1) << "Invalid speedAdd in SpeedPickup." << std::endl;
            this->speedAdd_ = 0.0f;
        }
    }

    /**
    @brief
        Sets the SpeedMultiply
    @param speedAdd
        The multiplied Speed
    */
    void SpeedPickup::setSpeedMultiply(float speedMultiply)
    {
        if(speedMultiply != 0)
        {
            this->speedMultiply_ = speedMultiply;
        }
        else
        {
            COUT(1) << "Invalid speedMultiply in SpeedPickup." << std::endl;
            this->speedMultiply_ = 1.0f;
        }
    }

    void SpeedPickup::PickupTimerCallBack(void) {
            Engine* engine = this->carrierToEngineHelper();
            if(engine == NULL) //!< If the PickupCarrier is no Pawn, then this pickup is useless and therefore is destroyed.
                this->destroy();

            engine->setSpeedAdd(0.0f);
            engine->setSpeedMultiply(1.0f);
    }
}
