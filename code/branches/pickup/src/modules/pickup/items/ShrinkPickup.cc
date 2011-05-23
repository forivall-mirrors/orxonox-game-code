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
 *      Sandro Sgier
 *   Co-authors:
 *      ...
 *
 */


/**
    @file ShrinkPickup.cc
    @brief Implementation of the HealthPickup class.
*/


#include "ShrinkPickup.h"

#include <sstream>
#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

#include "pickup/PickupIdentifier.h"
#include "worldentities/pawns/Pawn.h"

#include "weaponsystem/WeaponSlot.h"
#include "weaponsystem/Weapon.h"
#include "worldentities/CameraPosition.h"

namespace orxonox
{
    CreateFactory(ShrinkPickup);

		/**
    @brief
        Constructor: Initializes the Pickup.
    */
    ShrinkPickup::ShrinkPickup(BaseObject* creator) : Pickup(creator)
    {
        RegisterObject(ShrinkPickup);

        this->initialize();
        isActive_ = false;
        isTerminating_ = false;
    }

    ShrinkPickup::~ShrinkPickup()
    {

    }

    void ShrinkPickup::initializeIdentifier(void)
    {
        std::stringstream stream;
        stream << this->getShrinkFactor();
        std::string type1 = "shrinkFactor";
        std::string val1 = stream.str();
        this->pickupIdentifier_->addParameter(type1, val1);

        stream.clear();
        stream << this->getDuration();
        std::string val2 = stream.str();
        std::string type2 = "duration";
        this->pickupIdentifier_->addParameter(type2, val2);

        stream.clear();
        stream << this->getShrinkSpeed();
        std::string val3 = stream.str();
        std::string type3 = "shrinkSpeed";
        this->pickupIdentifier_->addParameter(type3, val3);
    }

   /**
    @brief
        Method for creating a HealthPickup object through XML.
    */
    void ShrinkPickup::XMLPort(Element& xmlelement, orxonox::XMLPort::Mode mode)
    {
        SUPER(ShrinkPickup, XMLPort, xmlelement, mode);

        XMLPortParam(ShrinkPickup, "shrinkFactor", setShrinkFactor, getShrinkFactor, xmlelement, mode);
        XMLPortParam(ShrinkPickup, "duration", setDuration, getDuration, xmlelement, mode);
        XMLPortParam(ShrinkPickup, "shrinkSpeed", setShrinkSpeed, getShrinkSpeed, xmlelement, mode);

        this->initializeIdentifier();
    }

    /**
    @brief
        Sets the shrinking factor.
    @param factor
        The factor.
    */
    void ShrinkPickup::setShrinkFactor(float factor)
    {
        this->shrinkFactor_ = factor;
    }

    /**
    @brief
        Sets the duration.
    @param duration
        The duration.
    */
    void ShrinkPickup::setDuration(float duration)
    {
        this->duration_ = duration;
    }

    /**
    @brief
        Sets the shrinking speed.
    @param speed
        The speed.
    */
    void ShrinkPickup::setShrinkSpeed(float speed)
    {
        this->shrinkSpeed_ = speed;
    }

    void ShrinkPickup::initialize(void)
    {
        this->addTarget(ClassIdentifier<Pawn>::getIdentifier());
    }

		/**
    @brief
        Prepares for shrinking (collecting several informations).
    */
    void ShrinkPickup::changedUsed(void)
    {
        SUPER(ShrinkPickup, changedUsed);

        if(this->isUsed())
        {
            this->pawn = this->carrierToPawnHelper();
            if(pawn == NULL) // If the PickupCarrier is no Pawn, then this pickup is useless and therefore is destroyed.
                this->Pickupable::destroy();

            //Collect scaling information.
            defaultScale_ = this->pawn->getScale3D();
            defaultMass_ = this->pawn->getMass();

            smallScale_ = defaultScale_ / shrinkFactor_;
            smallMass_ = defaultMass_ / shrinkFactor_;

            actualScale_ = defaultScale_;
            actualMass_ = defaultMass_;

            cameraPositions_ = this->pawn->getCameraPositions();
            size_ = cameraPositions_.size();
            isActive_ = true;    //start shrinking now.
            durationTimer.setTimer(duration_, false, createExecutor(createFunctor(&ShrinkPickup::terminate, this)));    //Set timer for termination.
        }
    }

	/**
    @brief
        Updates the scales of the ship.
    @param dt
        Time since last call.
    */
    void ShrinkPickup::tick(float dt)
    {
        if(isActive_ == true && actualScale_ > smallScale_)    //if the ship has not reached the target scale, continue shrinking
        {
            float factor_ = 1 + dt*shrinkSpeed_;

            actualScale_ /= factor_;
            actualMass_ /= factor_;

            this->pawn->setScale3D(actualScale_);
            this->pawn->setMass(actualMass_);

            for(int index = 0; index < size_; index++)
            {
                CameraPosition* cameraPos_ = this->pawn->getCameraPosition(index);
                if(cameraPos_ == NULL)
                continue;
                cameraPos_->setPosition(cameraPos_->getPosition()*factor_);
            }
        }
        else isActive_ = false;

        if(isTerminating_ == true && actualScale_ < defaultScale_)    //grow until the ship reaches its default scale.
        {
            float factor_ = 1 + dt*shrinkSpeed_;

            actualScale_ *= factor_;
            actualMass_ *= factor_;

            this->pawn->setScale3D(actualScale_);
            this->pawn->setMass(actualMass_);

            for(int index = 0; index < size_; index++)
            {
                CameraPosition* cameraPos_ = this->pawn->getCameraPosition(index);
                if(cameraPos_ == NULL)
                continue;
                cameraPos_->setPosition(cameraPos_->getPosition()/factor_);
            }
        }
        else if(isTerminating_ == true)
            this->Pickupable::destroy();

    }

	/**
    @brief
        Initializes the termination.
    */
    void ShrinkPickup::terminate(void)
    {
        isActive_ = false;
        isTerminating_ = true;
        setUsed(false);
    }

    Pawn* ShrinkPickup::carrierToPawnHelper(void)
    {
        PickupCarrier* carrier = this->getCarrier();
        Pawn* pawn = dynamic_cast<Pawn*>(carrier);

        return pawn;
    }

	/**
    @brief
        Creates a duplicate of the input OrxonoxClass.
    @param item
        A pointer to the Orxonox class.
    */
    void ShrinkPickup::clone(OrxonoxClass*& item)
    {
        if(item == NULL)
            item = new ShrinkPickup(this);

        SUPER(ShrinkPickup, clone, item);
        ShrinkPickup* pickup = dynamic_cast<ShrinkPickup*>(item);
        pickup->setShrinkFactor(this->getShrinkFactor());
        pickup->setDuration(this->getDuration());
        pickup->setShrinkSpeed(this->getShrinkSpeed());

        pickup->initializeIdentifier();
    }
}
