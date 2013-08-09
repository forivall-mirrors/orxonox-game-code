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
 *      Benedict Simlinger
 *   Co-authors:
 *      ...
 *
 */

/**
    @file InvisiblePickup.cc
    @brief Implementation of the InvisiblePickup class.
*/

#include "InvisiblePickup.h"

#include <sstream>
//#include <OgreEntity.h>
//#include <OgreAnimationState.h>
#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

#include "worldentities/pawns/Pawn.h"

namespace orxonox
{

    CreateFactory(InvisiblePickup);

    /**
    @brief
        Constructor. Registers the object and initializes the member variables.
    */
    InvisiblePickup::InvisiblePickup(Context* context) : Pickup(context)
    {
        RegisterObject(InvisiblePickup);
        this->initialize();
    }

    /**
    @brief
        Destructor.
    */
    InvisiblePickup::~InvisiblePickup()
    {
    }

    /**
    @brief
    Initializes the member variables.
    */
    void InvisiblePickup::initialize(void)
    {
        this->duration_ = 0.0f;
        // Defines who is allowed to pick up the pickup.
        this->addTarget(ClassIdentifier<Pawn>::getIdentifier());
    }

    /**
    @brief
        Method for creating a HealthPickup object through XML.
    */
    void InvisiblePickup::XMLPort(Element& xmlelement, orxonox::XMLPort::Mode mode)
    {
        SUPER(InvisiblePickup, XMLPort, xmlelement, mode);
        XMLPortParam(InvisiblePickup, "duration", setDuration, getDuration, xmlelement, mode);
    }

    /**
    @brief
        Is called when the pickup has transited from used to unused or the other way around.
    */
    void InvisiblePickup::changedUsed(void)
    {
        SUPER(InvisiblePickup, changedUsed);

        // If the pickup has transited to used.
        if (this->isUsed())
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
                    this->durationTimer_.setTimer(this->getDuration(), false, createExecutor(createFunctor(&InvisiblePickup::pickupTimerCallback, this)));
                }
            }

            this->setInvisible(true);

        }
        else
        {
            this->setInvisible(false);

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
        Sets the invisibility.
    @param invisibility
        The invisibility.
    */
    bool InvisiblePickup::setInvisible(bool invisibility)
    {
        Pawn* pawn = this->carrierToPawnHelper();
        if(pawn == NULL)
            return false;

        pawn->setVisible(!invisibility);
        //TODO: Invisibility should imply radar invisibility as well, thus this should be solved in Pawn.
        pawn->setRadarVisibility(!invisibility);

// Test to change Material at runtime!

//      Ogre::MaterialPtr mat = this->mesh_.getEntity()->getSubEntity(0)->getMaterial();
//      mat->setDiffuse(0.4, 0.3, 0.1, 0.1);
//      mat->setAmbient(0.3, 0.7, 0.8);
//      mat->setSpecular(0.5, 0.5, 0.5, 0.1);
//      Ogre::SceneBlendType sbt = Ogre::SBT_ADD;
//
//      mat->setSceneBlending(sbt);

        return true;
    }

    /**
    @brief
        Helper to transform the PickupCarrier to a Pawn, and throw an error message if the conversion fails.
    @return
        A pointer to the Pawn, or NULL if the conversion failed.
    */
    Pawn* InvisiblePickup::carrierToPawnHelper(void)
    {
        PickupCarrier* carrier = this->getCarrier();
        Pawn* pawn = orxonox_cast<Pawn*>(carrier);

        if(pawn == NULL)
        {
            orxout(internal_error, context::pickups) << "Invalid PickupCarrier in InvisiblePickup." << endl;
        }
        return pawn;
    }

    /**
    @brief
        Sets the time the InvisibilityPickup will last.
    @param duration
        The duration in seconds.
    */
    void InvisiblePickup::setDuration(float duration)
    {
        if(duration >= 0.0f)
        {
            this->duration_ = duration;
        }
        else
        {
            orxout(internal_error, context::pickups) << "Invalid duration in InvisiblePickup." << endl;
            this->duration_ = 0.0f;
        }
    }

    /**
    @brief
        Helper method. Is called by the Timer as soon as it expires.
    */
    void InvisiblePickup::pickupTimerCallback(void)
    {
        this->setUsed(false);
    }

}
