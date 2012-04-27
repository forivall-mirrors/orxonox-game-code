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

#include "DamageBoostPickup.h"

#include <sstream>
#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

#include "pickup/PickupIdentifier.h"
#include "worldentities/pawns/SpaceShip.h"

namespace orxonox
{
    CreateFactory(DamageBoostPickup);

    /**
    @brief
        Constructor. Registers the object and initializes the member variables.
    */
    DamageBoostPickup::DamageBoostPickup(BaseObject* creator) : Pickup(creator)
    {
        RegisterObject(DamageBoostPickup);

        this->initialize();
    }

    /**
    @brief
        Destructor.
    */
    DamageBoostPickup::~DamageBoostPickup()
    {
    }

    /**
    @brief
        Initializes the member variables.
    */
    void DamageBoostPickup::initialize(void)
    {
        this->duration_ = 0.0f;
        this->damageMultiplier_ = 20.0f;
        //Defines who is allowed to pick up the pickup.
        this->addTarget(ClassIdentifier<SpaceShip>::getIdentifier());
    }

    /**
    @brief
        Initializes the PickupIdentifier of this pickup.
    */
    void DamageBoostPickup::initializeIdentifier(void)
    {
        std::stringstream stream;
        stream << this->getDuration();
        std::string type1 = "duration";
        std::string val1 = stream.str();
        this->pickupIdentifier_->addParameter(type1, val1);

        stream.clear();
        stream << this->damageMultiplier_;
        std::string type2 = "damageMultiplier";
        std::string val2 = stream.str();
        this->pickupIdentifier_->addParameter(type2, val2);

    }

    /**
    @brief
        Method for creating a DamageBoostPickup object through XML.
    */
    void DamageBoostPickup::XMLPort(Element& xmlelement, orxonox::XMLPort::Mode mode)
    {
        SUPER(DamageBoostPickup, XMLPort, xmlelement, mode);

        XMLPortParam(DamageBoostPickup, "duration", setDuration, getDuration, xmlelement, mode);
        XMLPortParam(DamageBoostPickup, "damageMultiplier", setDefaultDamageMultiplier, getDefaultDamageMultiplier, xmlelement, mode);

        this->initializeIdentifier();
    }

    /**
    @brief
        Is called when the pickup has transited from used to unused or the other way around.
    */
    void DamageBoostPickup::changedUsed(void)
    {
        SUPER(DamageBoostPickup, changedUsed);

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
                    this->durationTimer_.setTimer(this->getDuration(), false, createExecutor(createFunctor(&DamageBoostPickup::pickupTimerCallback, this)));
                }
            }
            // NOTE: commented this since its use was not apparent
            // ship->getCreator();

            this->olddamageMultiplier_ = ship->getDamageMultiplier();
            ship->setDamageMultiplier( this->damageMultiplier_ );
        }
        else
        {

            // We destroy the pickup if either, the pickup has activationType immediate and durationType once or it has durationType continuous and the duration was exceeded.
            if((!this->isContinuous() && this->isImmediate()) || (this->isContinuous() && !this->durationTimer_.isActive() && this->durationTimer_.getRemainingTime() == this->getDuration()))
            {
                this->Pickupable::destroy();
                ship->setDamageMultiplier ( this->olddamageMultiplier_);
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
    SpaceShip* DamageBoostPickup::carrierToSpaceShipHelper(void)
    {
        PickupCarrier* carrier = this->getCarrier();
        SpaceShip* ship = dynamic_cast<SpaceShip*>(carrier);

        if(ship == NULL)
        {
            orxout(internal_error, context::pickups) << "Invalid PickupCarrier in DamageBoostPickup." << endl;
        }

        return ship;
    }

    /**
    @brief
        Creates a duplicate of the input OrxonoxClass.
    @param item
        A pointer to the Orxonox class.
    */
    void DamageBoostPickup::clone(OrxonoxClass*& item)
    {
        if(item == NULL)
            item = new DamageBoostPickup(this);

        SUPER(DamageBoostPickup, clone, item);

        DamageBoostPickup* pickup = dynamic_cast<DamageBoostPickup*>(item);
        pickup->setDuration(this->getDuration());
        

        pickup->initializeIdentifier();
    }

    /**
    @brief
        Sets the duration for which the DamageBoostPickup stays active.
    @param duration
        The duration in seconds.
    */
    void DamageBoostPickup::setDuration(float duration)
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

   /* void DamageBoostPickup::setDamageBoost(float damageBoost)
       {

       }
    void DamageBoostPickup::setDamageSave(float damageSave)
    {

    }
    */

    void DamageBoostPickup::pickupTimerCallback(void)
    {
        this->setUsed(false);
    }

}

