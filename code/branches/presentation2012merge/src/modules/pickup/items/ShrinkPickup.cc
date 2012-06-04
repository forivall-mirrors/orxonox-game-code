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
    }

    ShrinkPickup::~ShrinkPickup()
    {

    }

    void ShrinkPickup::initialize(void)
    {
        this->addTarget(ClassIdentifier<Pawn>::getIdentifier());

        this->shrinkFactor_ = 5.0f;
        this->shrinkDuration_ = 5.0f;
        this->duration_ = 5.0f;

        this->isActive_ = false;
        this->isShrinking_ = false;
        this->isTerminating_ = false;

        this->timeRemainig_ = 0.0f;
        this->currentFactor_ = 1.0f;
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
        stream << this->getShrinkDuration();
        std::string val3 = stream.str();
        std::string type3 = "shrinkDuration";
        this->pickupIdentifier_->addParameter(type3, val3);
    }

   /**
    @brief
        Method for creating a ShrinkPickup object through XML.
    */
    void ShrinkPickup::XMLPort(Element& xmlelement, orxonox::XMLPort::Mode mode)
    {
        SUPER(ShrinkPickup, XMLPort, xmlelement, mode);

        XMLPortParam(ShrinkPickup, "shrinkFactor", setShrinkFactor, getShrinkFactor, xmlelement, mode);
        XMLPortParam(ShrinkPickup, "duration", setDuration, getDuration, xmlelement, mode);
        XMLPortParam(ShrinkPickup, "shrinkDuration", setShrinkDuration, getShrinkDuration, xmlelement, mode);

        this->initializeIdentifier();
    }

    /**
    @brief
        Prepares for shrinking.
    */
    void ShrinkPickup::changedUsed(void)
    {
        SUPER(ShrinkPickup, changedUsed);

        if(this->isUsed())
        {
            Pawn* pawn = this->carrierToPawnHelper();
            if(pawn == NULL) // If the PickupCarrier is no Pawn, then this pickup is useless and therefore is destroyed.
            {
                this->Pickupable::destroy();
                return;
            }

            this->currentFactor_ = 1.0f;
            this->timeRemainig_ = this->shrinkDuration_;

            this->isActive_ = true; // Start shrinking now.
            this->isShrinking_ = true;
            this->durationTimer_.setTimer(this->duration_, false, createExecutor(createFunctor(&ShrinkPickup::terminate, this))); //Set timer for termination.
        }
        if(!this->isUsed() && this->isActive_)
             this->isTerminating_ = true;
    }

    void ShrinkPickup::changedPickedUp(void)
    {
        SUPER(ShrinkPickup, changedPickedUp);

        if(!this->isPickedUp() && this->isActive_)
        {
            if(this->isShrinking_ || this->isTerminating_)
            {
                //TODO: Deploy particle effect.
                Pawn* pawn = this->carrierToPawnHelper();
                if(pawn == NULL) // If the PickupCarrier is no Pawn, then this pickup is useless and therefore is destroyed.
                    return;

                float factor = 1.0f/this->currentFactor_;

                pawn->setScale3D(pawn->getScale3D()*factor);
                pawn->setMass(pawn->getMass()*factor);

                // Iterate over all camera positions and inversely move the camera to create a shrinking sensation.
                const std::list< SmartPtr<CameraPosition> >& cameraPositions = pawn->getCameraPositions();
                int size = cameraPositions.size();
                for(int index = 0; index < size; index++)
                {
                    CameraPosition* cameraPos = pawn->getCameraPosition(index);
                    if(cameraPos == NULL)
                        continue;
                    cameraPos->setPosition(cameraPos->getPosition()/factor);
                }
                this->currentFactor_ = 1.0f;
                this->timeRemainig_ = this->shrinkDuration_;
                this->isActive_ = false;
                this->isShrinking_ = false;
                this->isTerminating_ = false;
            }
            else
            {
                //TODO: Deploy particle effect.
                Pawn* pawn = this->carrierToPawnHelper();
                if(pawn == NULL) // If the PickupCarrier is no Pawn, then this pickup is useless and therefore is destroyed.
                    return;

                pawn->setScale3D(pawn->getScale3D()*this->shrinkFactor_);
                pawn->setMass(pawn->getMass()*this->shrinkFactor_);

                // Iterate over all camera positions and inversely move the camera to create a shrinking sensation.
                const std::list< SmartPtr<CameraPosition> >& cameraPositions = pawn->getCameraPositions();
                int size = cameraPositions.size();
                for(int index = 0; index < size; index++)
                {
                    CameraPosition* cameraPos = pawn->getCameraPosition(index);
                    if(cameraPos == NULL)
                        continue;
                    cameraPos->setPosition(cameraPos->getPosition()/this->shrinkFactor_);
                }
                this->currentFactor_ = 1.0f;
                this->timeRemainig_ = this->shrinkDuration_;
                this->isActive_ = false;
            }
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
        if(this->isActive_)
        {
            if(this->isShrinking_)    // If the ship has not reached the target scale, continue shrinking
            {
                Pawn* pawn = this->carrierToPawnHelper();
                if(pawn == NULL) // If the PickupCarrier is no Pawn, then this pickup is useless and therefore is destroyed.
                {
                    this->Pickupable::destroy();
                    return;
                }

                this->timeRemainig_ -= dt;

                // Calculate the scaling factor by which the initial size would have to be scaled to have the current scale.
                float currentFactor = std::max(1 - (1-std::max(this->timeRemainig_, 0.0f)/this->shrinkDuration_)*(1-1/this->shrinkFactor_), 1/this->shrinkFactor_);
                // Calculate the factor by which the previous size has to be scaled to be the current scale.
                float factor = currentFactor/this->currentFactor_;
                this->currentFactor_ = currentFactor;

                // Stop shrinking if the desired size is reached.
                if(this->timeRemainig_ <= 0.0f)
                {
                    this->timeRemainig_ = this->shrinkDuration_; // Reset the time remaining for when we start to grow the ship again.
                    this->currentFactor_ = 1/this->shrinkFactor_;
                    this->isShrinking_ = false;
                }

                pawn->setScale3D(pawn->getScale3D()*factor);
                pawn->setMass(pawn->getMass()*factor);

                // Iterate over all camera positions and inversely move the camera to create a shrinking sensation.
                const std::list< SmartPtr<CameraPosition> >& cameraPositions = pawn->getCameraPositions();
                int size = cameraPositions.size();
                for(int index = 0; index < size; index++)
                {
                    CameraPosition* cameraPos = pawn->getCameraPosition(index);
                    if(cameraPos == NULL)
                        continue;
                    cameraPos->setPosition(cameraPos->getPosition()/factor);
                }

            }
            else if(this->isTerminating_)    // Grow until the ship reaches its default scale.
            {
                Pawn* pawn = this->carrierToPawnHelper();
                if(pawn == NULL) // If the PickupCarrier is no Pawn, then this pickup is useless and therefore is destroyed.
                    this->Pickupable::destroy();

                this->timeRemainig_ -= dt;

                // Calculate the scaling factor by which the initial size would have to be scaled to have the current scale.
                float currentFactor = std::min(1/this->shrinkFactor_ + (1-std::max(this->timeRemainig_, 0.0f)/this->shrinkDuration_)*(1-1/this->shrinkFactor_), 1.0f);
                // Calculate the factor by which the previous size has to be scaled to be the current scale.
                float factor = currentFactor/this->currentFactor_;
                this->currentFactor_ = currentFactor;

                bool destroy = false;

                // Stop shrinking if the desired size is reached.
                if(this->timeRemainig_ <= 0.0f)
                {
                    this->timeRemainig_ = shrinkDuration_; // Reset the time remaining for when we start to grow the ship again.
                    this->currentFactor_ = 1.0f;
                    this->isTerminating_ = false;
                    this->isActive_ = false;
                    destroy = true;
                }

                pawn->setScale3D(pawn->getScale3D()*factor);
                pawn->setMass(pawn->getMass()*factor);

                // Iterate over all camera positions and inversely move the camera to create a shrinking sensation.
                const std::list< SmartPtr<CameraPosition> >& cameraPositions = pawn->getCameraPositions();
                int size = cameraPositions.size();
                for(int index = 0; index < size; index++)
                {
                    CameraPosition* cameraPos = pawn->getCameraPosition(index);
                    if(cameraPos == NULL)
                        continue;
                    cameraPos->setPosition(cameraPos->getPosition()/factor);
                }

                if(destroy)
                    this->Pickupable::destroy();
            }
        }
    }

    /**
    @brief
        Initializes the termination.
    */
    void ShrinkPickup::terminate(void)
    {
        this->setUsed(false);
    }

    Pawn* ShrinkPickup::carrierToPawnHelper(void)
    {
        PickupCarrier* carrier = this->getCarrier();
        Pawn* pawn = orxonox_cast<Pawn*>(carrier);

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
        ShrinkPickup* pickup = orxonox_cast<ShrinkPickup*>(item);
        pickup->setShrinkFactor(this->getShrinkFactor());
        pickup->setDuration(this->getDuration());
        pickup->setShrinkDuration(this->getShrinkDuration());

        pickup->initializeIdentifier();
    }
}
