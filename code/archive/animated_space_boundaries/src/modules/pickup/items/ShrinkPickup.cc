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
        this->shrinkFactor_ = 5.0f;
        this->shrinkSpeed_ = 5.0f;
        this->duration_ = 5.0f;
        this->isActive_ = false;
        this->isTerminating_ = false;

        this->size_ = 0;
        this->defaultCameraPos_ = 0.0f;
        this->defaultScale_ = Vector3::UNIT_SCALE;
        this->actualScale_ = Vector3::UNIT_SCALE;
        this->smallScale_ = Vector3::UNIT_SCALE;
        this->defaultMass_ = 1.0f;
        this->actualMass_ = 1.0f;
        this->smallMass_ = 1.0f;
        this->pawn_ = NULL;
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
        Method for creating a ShrinkhPickup object through XML.
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
            this->pawn_ = this->carrierToPawnHelper();
            if(this->pawn_ == NULL) // If the PickupCarrier is no Pawn, then this pickup is useless and therefore is destroyed.
                this->Pickupable::destroy();

            //Collect scaling information.
            this->defaultScale_ = this->pawn_->getScale3D();
            this->defaultMass_ = this->pawn_->getMass();

            this->smallScale_ = this->defaultScale_ / this->shrinkFactor_;
            this->smallMass_ = this->defaultMass_ / this->shrinkFactor_;

            this->actualScale_ = this->defaultScale_;
            this->actualMass_ = this->defaultMass_;

            this->cameraPositions_ = this->pawn_->getCameraPositions();
            this->size_ = this->cameraPositions_.size();
            this->isActive_ = true;    //start shrinking now.
            this->durationTimer_.setTimer(this->duration_, false, createExecutor(createFunctor(&ShrinkPickup::terminate, this)));    //Set timer for termination.
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
        if(this->isActive_ == true && this->actualScale_ > this->smallScale_)    //if the ship has not reached the target scale, continue shrinking
        {
            float factor = 1 + dt*this->shrinkSpeed_;

            this->actualScale_ /= factor;
            this->actualMass_ /= factor;

            this->pawn_->setScale3D(this->actualScale_);
            this->pawn_->setMass(this->actualMass_);

            for(int index = 0; index < this->size_; index++)
            {
                CameraPosition* cameraPos = this->pawn_->getCameraPosition(index);
                if(cameraPos == NULL)
                continue;
                cameraPos->setPosition(cameraPos->getPosition()*factor);
            }
        }
        else this->isActive_ = false;

        if(this->isTerminating_ == true && this->actualScale_ < this->defaultScale_)    //grow until the ship reaches its default scale.
        {
            float factor = 1 + dt*this->shrinkSpeed_;

            this->actualScale_ *= factor;
            this->actualMass_ *= factor;

            this->pawn_->setScale3D(this->actualScale_);
            this->pawn_->setMass(this->actualMass_);

            for(int index = 0; index < this->size_; index++)
            {
                CameraPosition* cameraPos = this->pawn_->getCameraPosition(index);
                if(cameraPos == NULL)
                continue;
                cameraPos->setPosition(cameraPos->getPosition()/factor);
            }
        }
        else if(this->isTerminating_ == true)
            this->Pickupable::destroy();

    }

    /**
    @brief
        Initializes the termination.
    */
    void ShrinkPickup::terminate(void)
    {
        this->isActive_ = false;
        this->isTerminating_ = true;
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
