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
 *      Nino Weingart
 *   Co-authors:
 *      ...
 *
 */

/**
    @file FlagPickup.cc
    @brief Implementation of the FlagPickup class.
*/

#include "FlagPickup.h"

#include <sstream>
#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

#include "pickup/PickupIdentifier.h"

namespace orxonox
{

    CreateFactory(FlagPickup);

    /**
    @brief
        Constructor. Registers the object and initializes the member variables.
    */
    FlagPickup::FlagPickup(BaseObject* creator) : Pickup(creator)
    {
        RegisterObject(FlagPickup);

        this->initialize();
    }

    /**
    @brief
        Destructor.
    */
    FlagPickup::~FlagPickup()
    {

    }

    /**
    @brief
        Initializes the member variables.
    */
    void FlagPickup::initialize(void)
    {
        this->flagType_ = 0;

        this->addTarget(ClassIdentifier<Pawn>::getIdentifier());
    }

    /**
    @brief
        Initializes the PickupIdentifier of this pickup.
    */
    void FlagPickup::initializeIdentifier(void)
    {
    	std::stringstream stream;
        int type = this->getFlagType();
        stream << type;
		std::string val = stream.str();
		
        std::string type1 = "flagType";
        this->pickupIdentifier_->addParameter(type1, val);
    }

    /**
    @brief
        Method for creating a FlagPickup object through XML.
    */
    void FlagPickup::XMLPort(Element& xmlelement, orxonox::XMLPort::Mode mode)
    {
        SUPER(FlagPickup, XMLPort, xmlelement, mode);

        XMLPortParam(FlagPickup, "flagType", setFlagType, getFlagType, xmlelement, mode);
        //XMLPortParam(FlagPickup, "teamNumber", setTeamNumber, getTeamNumber, xmlelement, mode);

        this->initializeIdentifier();
    }


    /**
    @brief
        Get the flag type of this pickup.
    @return
        Returns the falg type as a string.
    */
    const int FlagPickup::getFlagType(void) const
    {
		return this->flagType_;
	}

    /**
    @brief
        Set the type of the HealthPickup.
    @param type
        The type as a string.
    */
    void FlagPickup::setFlagType(int type)
    {
        if(type<3){
        	this->flagType_ = type;
        }
        
        else{
            orxout(internal_error, context::pickups) << "Invalid flagType '" << type << "' in FlagPickup." << endl;
    	}
    }

    /**
       @brief
           Is called when the pickup has transited from used to unused or the other way around.
       */
       void FlagPickup::changedUsed(void)
       {
           SUPER(FlagPickup, changedUsed);

           Pawn* pawn = this->carrierToPawnHelper();

           if(pawn == NULL) // If the PickupCarrier is no Pawn, then this pickup is useless and therefore is destroyed.
               this->Pickupable::destroy();

           // If the pickup has transited to used.
           if(this->isUsed())
           {
           	 this->bPickedUp_ = true;
           	 this->pickedUpBy_ = pawn;
           }
       }

       /**
       @brief
           Helper to transform the PickupCarrier to a Pawn, and throw an error message if the conversion fails.
       @return
           A pointer to the Pawn, or NULL if the conversion failed.
       */
       Pawn* FlagPickup::carrierToPawnHelper(void)
       {
           PickupCarrier* carrier = this->getCarrier();
           Pawn* pawn = dynamic_cast<Pawn*>(carrier);

           if(pawn == NULL)
               orxout(internal_error, context::pickups) << "Invalid PickupCarrier in HealthPickup." << endl;

           return pawn;
       }



	void FlagPickup::tick(float dt)
	   {
		   SUPER(FlagPickup, tick, dt);

	       Pawn* pawn = this->carrierToPawnHelper();

		   if(pawn->isAlive() && pawn->hasFlag()){
               this->Pickupable::destroy();
		   }

	   }
}