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
 *      Lukas Gasser
 *   Co-authors:
 *      ...
 *
 */

/**
    @file DronePickup.cc
    @brief Implementation of the DronePickup class.
*/

#include "DronePickup.h"

#include <sstream>
#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

#include "controllers/DroneController.h"
#include "worldentities/Drone.h"
#include "worldentities/pawns/Pawn.h"

namespace orxonox
{

    CreateFactory(DronePickup);

    /**
    @brief
        Constructor. Registers the object and initializes the member variables.
    */
    DronePickup::DronePickup(BaseObject* creator) : Pickup(creator)
    {
        RegisterObject(DronePickup);

        this->initialize();
    }

    /**
    @brief
        Destructor.
    */
    DronePickup::~DronePickup()
    {

    }

    /**
    @brief
        Initializes the member variables.
    */
    void DronePickup::initialize(void)
    {
        this->addTarget(ClassIdentifier<Pawn>::getIdentifier());
        this->setDurationType(pickupDurationType::once);
        this->droneTemplate_ = "";
    }

    /**
    @brief
        Method for creating a DronePickup object through XML.
    */
    void DronePickup::XMLPort(Element& xmlelement, orxonox::XMLPort::Mode mode)
    {
        SUPER(DronePickup, XMLPort, xmlelement, mode);
        XMLPortParam(DronePickup, "droneTemplate", setDroneTemplate, getDroneTemplate, xmlelement, mode);
    }

    /**
    @brief
        Set the droneTemplate.
    @param templatename
        The name of the Template to e set.
    */
    void DronePickup::setDroneTemplate(const std::string& templatename){
        droneTemplate_ = templatename;
    }

    /**
    @brief
        Get the name of the droneTemplate.
    @return
        Returns the name of the droneTemplate.
    */
    const std::string& DronePickup::getDroneTemplate() const
    {
        return droneTemplate_;
    }

    /**
    @brief
        Is called when the pickup has transited from used to unused or the other way around.
    */
    void DronePickup::changedUsed(void)
    {
        SUPER(DronePickup, changedUsed);

        // If the pickup has transited to used.
        if(this->isUsed())
        {

                Pawn* pawn = this->carrierToPawnHelper();
                if(pawn == NULL) // If the PickupCarrier is no Pawn, then this pickup is useless and therefore is destroyed.
                    this->Pickupable::destroy();

                //Attach to pawn
                Drone* drone = new Drone(pawn->getCreator()); // this is neccessary because the projectiles fired need a valid creator for the particlespawner (when colliding against something)
                drone->addTemplate(this->getDroneTemplate());

                Controller* controller = drone->getController();
                DroneController* droneController = orxonox_cast<DroneController*>(controller);
                if(droneController != NULL)
                {
                    droneController->setOwner(pawn);
                }

                Vector3 spawnPosition = pawn->getWorldPosition() + Vector3(30,0,-30);
                drone->setPosition(spawnPosition);

                // The pickup has been used up.
                this->setUsed(false);
        }
        else
        {
            // If either the pickup can only be used once or it is continuous and used up, it is destroyed upon setting it to unused.
            if(this->isOnce() || (this->isContinuous() ))
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
    Pawn* DronePickup::carrierToPawnHelper(void)
    {
        PickupCarrier* carrier = this->getCarrier();
        Pawn* pawn = orxonox_cast<Pawn*>(carrier);

        if(pawn == NULL)
        {
            orxout(internal_error, context::pickups) << "Invalid PickupCarrier in DronePickup." << endl;
        }

        return pawn;
    }

    /**
    @brief
        Creates a duplicate of the input OrxonoxClass.
    @param item
        A pointer to the Orxonox class.
    */
    void DronePickup::clone(OrxonoxClass*& item)
    {
        if(item == NULL)
            item = new DronePickup(this);

        SUPER(DronePickup, clone, item);

        DronePickup* pickup = orxonox_cast<DronePickup*>(item);
        pickup->setDroneTemplate(this->getDroneTemplate());
    }
}
