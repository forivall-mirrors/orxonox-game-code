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
    @file HealthPickup.cc
    @brief Implementation of the HealthPickup class.
*/

#include "HealthPickup.h"

#include <sstream>
#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

#include "worldentities/pawns/Pawn.h"

namespace orxonox
{

    /*static*/ const std::string HealthPickup::healthTypeLimited_s = "limited";
    /*static*/ const std::string HealthPickup::healthTypeTemporary_s = "temporary";
    /*static*/ const std::string HealthPickup::healthTypePermanent_s = "permanent";

    RegisterClass(HealthPickup);

    /**
    @brief
        Constructor. Registers the object and initializes the member variables.
    */
    HealthPickup::HealthPickup(Context* context) : Pickup(context)
    {
        RegisterObject(HealthPickup);

        this->initialize();
    }

    /**
    @brief
        Destructor.
    */
    HealthPickup::~HealthPickup()
    {

    }

    /**
    @brief
        Initializes the member variables.
    */
    void HealthPickup::initialize(void)
    {
        this->health_ = 0.0f;
        this->healthRate_ = 0.0f;
        this->healthType_ = pickupHealthType::limited;
        this->maxHealthSave_ = 0.0f;
        this->maxHealthOverwrite_ = 0.0f;

        this->addTarget(ClassIdentifier<Pawn>::getIdentifier());
    }

    /**
    @brief
        Method for creating a HealthPickup object through XML.
    */
    void HealthPickup::XMLPort(Element& xmlelement, orxonox::XMLPort::Mode mode)
    {
        SUPER(HealthPickup, XMLPort, xmlelement, mode);

        XMLPortParam(HealthPickup, "health", setHealth, getHealth, xmlelement, mode);
        XMLPortParam(HealthPickup, "healthRate", setHealthRate, getHealthRate, xmlelement, mode);
        XMLPortParam(HealthPickup, "healthType", setHealthTypeAsString, getHealthTypeAsString, xmlelement, mode);

        if(!this->isContinuous())
            this->setHealthRate(0.0f); // TODO: this logic should be inside tick(), not in XMLPort
    }

    /**
    @brief
        Is called every tick.
        Does all the continuous stuff of this HealthPickup.
    @param dt
        The duration of the last tick.
    */
    void HealthPickup::tick(float dt)
    {
        SUPER(HealthPickup, tick, dt);

        if(this->isContinuous() && this->isUsed())
        {
            Pawn* pawn = this->carrierToPawnHelper();
            if(pawn == NULL) // If the PickupCarrier is no Pawn, then this pickup is useless and therefore is destroyed.
                this->Pickupable::destroy();

            // Calculate the health that is added this tick.
            float health = dt*this->getHealthRate();
            if(health > this->getHealth())
                health = this->getHealth();
            // Calculate the health the Pawn will have once the health is added.
            float fullHealth = pawn->getHealth() + health;
            this->setHealth(this->getHealth()-health);

            switch(this->getHealthType())
            {
                case pickupHealthType::permanent:
                    if(pawn->getMaxHealth() < fullHealth)
                        pawn->setMaxHealth(fullHealth);
                case pickupHealthType::limited:
                    pawn->addHealth(health);
                    break;
                case pickupHealthType::temporary:
                    if(pawn->getMaxHealth() > fullHealth)
                    {
                        this->maxHealthSave_ = pawn->getMaxHealth();
                        this->maxHealthOverwrite_ = fullHealth;
                        pawn->setMaxHealth(fullHealth);
                    }
                    pawn->addHealth(health);
                    break;
                default:
                    orxout(internal_error, context::pickups) << "Invalid healthType in HealthPickup." << endl;
            }

            // If all health has been transferred.
            if(this->getHealth() == 0.0f)
            {
                this->setUsed(false);
            }
        }
    }

    /**
    @brief
        Is called when the pickup has transited from used to unused or the other way around.
    */
    void HealthPickup::changedUsed(void)
    {
        SUPER(HealthPickup, changedUsed);

        // If the pickup has transited to used.
        if(this->isUsed())
        {
            if(this->isOnce())
            {
                Pawn* pawn = this->carrierToPawnHelper();
                if(pawn == NULL) // If the PickupCarrier is no Pawn, then this pickup is useless and therefore is destroyed.
                    this->Pickupable::destroy();

                float health = 0.0f;
                switch(this->getHealthType())
                {
                    case pickupHealthType::permanent:
                        health = pawn->getHealth()+this->getHealth();
                        if(pawn->getMaxHealth() < health)
                            pawn->setMaxHealth(health);
                    case pickupHealthType::limited:
                        pawn->addHealth(this->getHealth());
                        break;
                    case pickupHealthType::temporary:
                        health = pawn->getHealth()+this->getHealth();
                        if(pawn->getMaxHealth() < health)
                        {
                            this->maxHealthSave_ = pawn->getMaxHealth();
                            this->maxHealthOverwrite_ = health;
                            pawn->setMaxHealth(health);
                        }
                        pawn->addHealth(this->getHealth());
                        break;
                    default:
                        orxout(internal_error, context::pickups) << "Invalid healthType in HealthPickup." << endl;
                }

                // The pickup has been used up.
                this->setUsed(false);
            }
        }
        else
        {
            if(this->getHealthType() == pickupHealthType::temporary)
            {
                PickupCarrier* carrier = this->getCarrier();
                Pawn* pawn = orxonox_cast<Pawn*>(carrier);

                if(pawn == NULL)
                {
                    orxout(internal_error, context::pickups) << "Something went horribly wrong in Health Pickup. PickupCarrier is '" << carrier->getIdentifier()->getName() << "' instead of Pawn." << endl;
                    this->Pickupable::destroy();
                    return;
                }

                if(pawn->getMaxHealth() == this->maxHealthOverwrite_)
                {
                    pawn->setMaxHealth(this->maxHealthSave_);
                    this->maxHealthOverwrite_ = 0;
                    this->maxHealthSave_ = 0;
                }
            }

            // If either the pickup can only be used once or it is continuous and used up, it is destroyed upon setting it to unused.
            if(this->isOnce() || (this->isContinuous() && this->getHealth() == 0.0f))
            {
                this->Pickupable::destroy();
            }
        }
    }

    /**
    @brief
        Helper to transform the PickupCarrier to a Pawn, and throw an error message if the conversion fails.
    @return
        A pointer to the Pawn, or NULL if the conversion failed.
    */
    Pawn* HealthPickup::carrierToPawnHelper(void)
    {
        PickupCarrier* carrier = this->getCarrier();
        Pawn* pawn = orxonox_cast<Pawn*>(carrier);

        if(pawn == NULL)
            orxout(internal_error, context::pickups) << "Invalid PickupCarrier in HealthPickup." << endl;

        return pawn;
    }

    /**
    @brief
        Get the health type of this pickup.
    @return
        Returns the health type as a string.
    */
    const std::string& HealthPickup::getHealthTypeAsString(void) const
    {
        switch(this->getHealthType())
        {
            case pickupHealthType::limited:
                return HealthPickup::healthTypeLimited_s;
            case pickupHealthType::temporary:
                return HealthPickup::healthTypeTemporary_s;
            case pickupHealthType::permanent:
                return HealthPickup::healthTypePermanent_s;
            default:
                orxout(internal_error, context::pickups) << "Invalid healthType in HealthPickup." << endl;
                return BLANKSTRING;
        }
    }

    /**
    @brief
        Sets the health.
    @param health
        The health.
    */
    void HealthPickup::setHealth(float health)
    {
        if(health >= 0.0f)
            this->health_ = health;
        else
        {
            orxout(internal_error, context::pickups) << "Invalid health '" << health << "' in HealthPickup. The health must be non.negative." << endl;
            this->health_ = 0.0f;
        }
    }

    /**
    @brief
        Set the rate at which health is transferred if the pickup is continuous.
    @param rate
        The rate.
    */
    void HealthPickup::setHealthRate(float rate)
    {
        if(rate >= 0.0f)
            this->healthRate_ = rate;
        else
            orxout(internal_error, context::pickups) << "Invalid healthRate '" << rate << "' in HealthPickup. The healthRate must be non-negative." << endl;
    }

    /**
    @brief
        Set the type of the HealthPickup.
    @param type
        The type as a string.
    */
    void HealthPickup::setHealthTypeAsString(const std::string& type)
    {
        if(type == HealthPickup::healthTypeLimited_s)
            this->setHealthType(pickupHealthType::limited);
        else if(type == HealthPickup::healthTypeTemporary_s)
            this->setHealthType(pickupHealthType::temporary);
        else if(type == HealthPickup::healthTypePermanent_s)
            this->setHealthType(pickupHealthType::permanent);
        else
            orxout(internal_error, context::pickups) << "Invalid healthType '" << type << "' in HealthPickup." << endl;
    }

}
