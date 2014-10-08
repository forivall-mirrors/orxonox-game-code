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

#include <sstream>
#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

#include "worldentities/pawns/Pawn.h"

namespace orxonox
{
    RegisterClass(ShieldPickup);

    /**
    @brief
        Constructor. Registers the object and initializes the member variables.
    */
    ShieldPickup::ShieldPickup(Context* context) : Pickup(context)
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
        this->shieldAbsorption_ = 0.0f;
        this->shieldHealth_ = 0.0f;

        this->addTarget(ClassIdentifier<Pawn>::getIdentifier());
    }

    /**
    @brief
        Method for creating a ShieldPickup object through XML.
    */
    void ShieldPickup::XMLPort(Element& xmlelement, orxonox::XMLPort::Mode mode)
    {
        SUPER(ShieldPickup, XMLPort, xmlelement, mode);

        XMLPortParam(ShieldPickup, "shieldhealth", setShieldHealth, getShieldHealth, xmlelement, mode);
        XMLPortParam(ShieldPickup, "shieldabsorption", setShieldAbsorption, getShieldAbsorption, xmlelement, mode);
        XMLPortParam(ShieldPickup, "duration", setDuration, getDuration, xmlelement, mode);
    }

    /**
    @brief
        Is called when the pickup has transited from used to unused or the other way around.
    */
    void ShieldPickup::changedUsed(void)
    {
        SUPER(ShieldPickup, changedUsed);

        Pawn* pawn = this->carrierToPawnHelper();
        if(pawn == NULL)
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
                    this->durationTimer_.setTimer(this->getDuration(), false, createExecutor(createFunctor(&ShieldPickup::pickupTimerCallback, this)));
                }
            }
            pawn->setShieldAbsorption(this->getShieldAbsorption());
            pawn->setShieldHealth(this->getShieldHealth());
        }
        else
        {
            pawn->setShieldAbsorption(0.0f);
            this->setShieldHealth(pawn->getShieldHealth());
            pawn->setShieldHealth(0.0f);

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
    Helper to transform the PickupCarrier to a Pawn, and throw an error message if the conversion fails.
    @return
    A pointer to the Pawn, or NULL if the conversion failed.
    */
    Pawn* ShieldPickup::carrierToPawnHelper(void)
    {
        PickupCarrier* carrier = this->getCarrier();
        Pawn* pawn = orxonox_cast<Pawn*>(carrier);

        if(pawn == NULL)
        {
            orxout(internal_error, context::pickups) << "Invalid PickupCarrier in ShieldPickup." << endl;
        }
        return pawn;
    }

    /**
    @brief
        Sets the duration.
    @param duration
        The duration in seconds.
    */
    void ShieldPickup::setDuration(float duration)
    {
        if(duration >= 0.0f)
        {
            this->duration_ = duration;
        }
        else
        {
            orxout(internal_error, context::pickups) << "Invalid duration in ShieldPickup." << endl;
            this->duration_ = 0.0f;
        }
    }

    /**
    @brief
        Sets the health of the shield.
    @param shieldHealth
        The shieldHealth.
    */
    void ShieldPickup::setShieldHealth(float shieldHealth)
    {
        if (shieldHealth>=0)
        {
            this->shieldHealth_=shieldHealth;
        }
        else
        {
            orxout(internal_error, context::pickups) << "Invalid Shieldhealth in ShieldPickup." << endl;
            this->shieldHealth_=0;
        }
    }

    /**
    @brief
        Sets the percentage the shield absorbs of the dealt damage.
    @param shieldAbsorption
        The shieldAbsorption. Has to be between 0 and 1.
    */
    void ShieldPickup::setShieldAbsorption(float shieldAbsorption)
    {
        if (shieldAbsorption>=0 && shieldAbsorption<=1)
        {
            this->shieldAbsorption_=shieldAbsorption;
        }
        else
        {
            orxout(internal_error, context::pickups) << "Invalid Absorption in ShieldPickup." << endl;
            this->shieldAbsorption_=0;
        }
    }

    /**
    @brief
        Helper method. Is called by the Timer as soon as it expires.
    */
    void ShieldPickup::pickupTimerCallback(void)
    {
        this->setUsed(false);
    }
}
