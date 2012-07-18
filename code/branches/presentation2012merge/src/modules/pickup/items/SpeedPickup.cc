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

#include <sstream>
#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

#include "worldentities/pawns/SpaceShip.h"

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
        this->duration_ = 0.0f;
        this->speedAdd_ = 0.0f;
        this->speedMultiply_ = 1.0f;

        this->addTarget(ClassIdentifier<SpaceShip>::getIdentifier());
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
    }

    /**
    @brief
        Is called when the pickup has transited from used to unused or the other way around.
    */
    void SpeedPickup::changedUsed(void)
    {
        SUPER(SpeedPickup, changedUsed);

        SpaceShip* ship = this->carrierToSpaceShipHelper();
        if(ship == NULL) // If the PickupCarrier is no SpaceShip, then this pickup is useless and therefore is destroyed.
            this->Pickupable::destroy();

        // If the pickup has transited to used.
        if(this->isUsed())
        {
            // If its durationType is continuous, we set a Timer to be reminded, when the time has run out.
            if(this->isContinuous())
            {
                if(!this->durationTimer_.isActive() && this->durationTimer_.getRemainingTime() > 0.0f)
                {
                    this->durationTimer_.unpauseTimer();
                }
                else
                {
                    this->durationTimer_.setTimer(this->getDuration(), false, createExecutor(createFunctor(&SpeedPickup::pickupTimerCallback, this)));
                }
            }

            ship->addSpeed(this->getSpeedAdd());
            ship->addSpeedFactor(this->getSpeedMultiply());
        }
        else
        {
            ship->addSpeed(-this->getSpeedAdd());
            ship->addSpeedFactor(1.0f/this->getSpeedMultiply());

            // We destroy the pickup if either, the pickup has activationType immediate and durationType once or it has durationType continuous and the duration was exceeded.
            if((!this->isContinuous() && this->isImmediate()) || (this->isContinuous() && !this->durationTimer_.isActive() && this->durationTimer_.getRemainingTime() == this->getDuration()))
            {
                this->Pickupable::destroy();
            }
            // We pause the Timer if the pickup is continuous and the duration is not yet exceeded,
            else if(this->isContinuous() && this->durationTimer_.isActive())
            {
                this->durationTimer_.pauseTimer();
            }
        }
    }

    /**
    @brief
        Helper to transform the PickupCarrier to a SpaceShip, and throw an error message if the conversion fails.
    @return
        A pointer to the SpaceShip, or NULL if the conversion failed.
    */
    SpaceShip* SpeedPickup::carrierToSpaceShipHelper(void)
    {
        PickupCarrier* carrier = this->getCarrier();
        SpaceShip* ship = orxonox_cast<SpaceShip*>(carrier);

        if(ship == NULL)
        {
            orxout(internal_error, context::pickups) << "Invalid PickupCarrier in SpeedPickup." << endl;
        }

        return ship;
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

        SpeedPickup* pickup = orxonox_cast<SpeedPickup*>(item);
        pickup->setDuration(this->getDuration());
        pickup->setSpeedAdd(this->getSpeedAdd());
        pickup->setSpeedMultiply(this->getSpeedMultiply());
    }

    /**
    @brief
        Sets the duration for which the SpeedPickup stays active.
    @param duration
        The duration in seconds.
    */
    void SpeedPickup::setDuration(float duration)
    {
        if(duration >= 0.0f)
        {
            this->duration_ = duration;
        }
        else
        {
            orxout(internal_error, context::pickups) << "Invalid duration in SpeedPickup." << endl;
            this->duration_ = 0.0f;
        }
    }

    /**
    @brief
        Sets the speedAdd, the value that is added to the speed of the Pawn.
    @param speedAdd
        The added speed.
    */
    void SpeedPickup::setSpeedAdd(float speedAdd)
    {
        if(speedAdd >= 0.0f)
        {
            this->speedAdd_ = speedAdd;
        }
        else
        {
            orxout(internal_error, context::pickups) << "Invalid speedAdd in SpeedPickup." << endl;
            this->speedAdd_ = 0.0f;
        }
    }

    /**
    @brief
        Sets the speedMultiply, the factor by which the speed of the Pawn is multiplied.
    @param speedMultiply
        The factor by which the speed is mutiplied.
    */
    void SpeedPickup::setSpeedMultiply(float speedMultiply)
    {
        if(speedMultiply != 0)
        {
            this->speedMultiply_ = speedMultiply;
        }
        else
        {
            orxout(internal_error, context::pickups) << "Invalid speedMultiply in SpeedPickup." << endl;
            this->speedMultiply_ = 1.0f;
        }
    }

    void SpeedPickup::pickupTimerCallback(void)
    {
        this->setUsed(false);
    }
}
