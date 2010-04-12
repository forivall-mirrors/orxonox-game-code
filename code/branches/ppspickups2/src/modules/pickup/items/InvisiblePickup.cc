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

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "util/StringUtils.h"

#include "worldentities/pawns/Pawn.h"
#include "pickup/PickupIdentifier.h"

#include <sstream>

namespace orxonox
{

    CreateFactory(InvisiblePickup);
    
    /**
    @brief
        Constructor. Registers the object and initializes the member variables.
    */
    InvisiblePickup::InvisiblePickup(BaseObject* creator) : Pickup(creator)
    {
        RegisterObject(InvisiblePickup);
	//! Defines who is allowed to pick up the pickup.
	this->initialize(); 
    }
    
    /**
    @brief
        Destructor.
    */
    InvisiblePickup::~InvisiblePickup()
    {        
    }
    
    
    void InvisiblePickup::initializeIdentifier(void)
    {
	std::stringstream stream;
	stream << this->getDuration();
	std::string type1 = "duration";
	std::string val1 = stream.str();
	this->pickupIdentifier_->addParameter(type1, val1);
    }
    
    /**
    @brief
    Initializes the member variables.
    */
    void InvisiblePickup::initialize(void)
    {
	this->duration_ = 0.0;
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
	 
	 this->initializeIdentifier();
    }
    
    /**
    @brief
        Is called when the pickup has transited from used to unused or the other way around.
    */
    void InvisiblePickup::changedUsed(void)
    {
        SUPER(InvisiblePickup, changedUsed);
	
        
        //! If the pickup is not picked up nothing must be done.
        if(!this->isPickedUp())
            return;
	if (this->isUsed())
	{
	    this->startPickupTimer(this->getDuration());
	    this->setInvisible(true);
	}
	else
	  this->setInvisible(false);
	
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
        Pawn* pawn = dynamic_cast<Pawn*>(carrier);
        
        if(pawn == NULL)
        {
            COUT(1) << "Invalid PickupCarrier in InvisiblePickup." << std::endl;
        }
        return pawn;
    }
    
    /**
    @brief
        Creates a duplicate of the input OrxonoxClass.
    @param item
        A pointer to the Orxonox class.
    */
    void InvisiblePickup::clone(OrxonoxClass*& item)
    {
        if(item == NULL)
            item = new InvisiblePickup(this);
        
        SUPER(InvisiblePickup, clone, item);
	
	InvisiblePickup* pickup = dynamic_cast<InvisiblePickup*>(item);
	pickup->setDuration(this->getDuration());
	pickup->initializeIdentifier();
    }
    
    /**
    @brief
        Sets the invisibility.
    @param health
        The invisibility.
    */
    bool InvisiblePickup::setInvisible(bool invisibility)
    {
      
      Pawn* pawn = this->carrierToPawnHelper();
      pawn->setVisible(!invisibility);
      return 0;
    }
    
    /**
    @brief
    Sets the duration.
    @param duration
    The duration
    */
    void InvisiblePickup::setDuration(float duration)
    {
	if(duration >= 0.0f)
	{
	    this->duration_ = duration;
	}
	else
	{
	    COUT(1) << "Invalid duration in InvisiblePickup." << std::endl;
	    this->duration_ = 0;
	}
    }
    
    void InvisiblePickup::PickupTimerCallBack(void)
    {
	this->setInvisible(false);
    }

}
