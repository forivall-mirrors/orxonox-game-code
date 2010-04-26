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
#include "worldentities/Drone.h"
#include "controllers/DroneController.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "util/StringUtils.h"

#include "worldentities/pawns/Pawn.h"
#include "pickup/PickupIdentifier.h"

#include <sstream>

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
        this->setDurationTypeDirect(pickupDurationType::once);
        this->droneTemplate_ = "";
    }
    
    /**
    @brief
        Initializes the PickupIdentifier of this pickup.
    */
    void DronePickup::initializeIdentifier(void)
    {
        std::string val = this->getDroneTemplate();
        std::string type = "droneTemplate";
        this->pickupIdentifier_->addParameter(type, val);
    }
    
    /**
    @brief
        Method for creating a DronePickup object through XML.
    */
    void DronePickup::XMLPort(Element& xmlelement, orxonox::XMLPort::Mode mode)
    {
        SUPER(DronePickup, XMLPort, xmlelement, mode);
        XMLPortParam(DronePickup, "droneTemplate", setDroneTemplate, getDroneTemplate, xmlelement, mode);
        
        this->initializeIdentifier();
    }
    
    void DronePickup::setDroneTemplate(std::string templatename){
        droneTemplate_ = templatename;
    } 
    
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
        
        //! If the pickup is not picked up nothing must be done.
        if(!this->isPickedUp())
            return;
        
        //! If the pickup has transited to used.
        if(this->isUsed())
        {

                Pawn* pawn = this->carrierToPawnHelper();
                if(pawn == NULL) //!< If the PickupCarrier is no Pawn, then this pickup is useless and therefore is destroyed.
                    this->destroy();
                
                //Attach to pawn
                Drone* drone = new Drone(this);
                drone->addTemplate(this->getDroneTemplate());
                pawn->attach(drone);
                Controller* controller = drone->getController();
                DroneController* droneController = dynamic_cast<DroneController*>(controller);
                if(droneController != NULL)
                {
                    droneController->setPawn(pawn);
                }

                //! The pickup has been used up.
                this->setUsed(false);
        }
        else
        {
            //! If either the pickup can only be used once or it is continuous and used up, it is destroyed upon setting it to unused.
            if(this->isOnce() || (this->isContinuous() ))
            {
                this->destroy();
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
        Pawn* pawn = dynamic_cast<Pawn*>(carrier);
        
        if(pawn == NULL)
        {
            COUT(1) << "Invalid PickupCarrier in DronePickup." << std::endl;
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
        
        DronePickup* pickup = dynamic_cast<DronePickup*>(item);
        pickup->setDroneTemplate(this->getDroneTemplate());
        
        pickup->initializeIdentifier();
    }
}
