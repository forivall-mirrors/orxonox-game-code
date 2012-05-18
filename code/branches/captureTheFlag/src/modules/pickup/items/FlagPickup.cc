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
#include "worldentities/pawns/Pawn.h"

namespace orxonox
{

    /*static*/ const std::string FlagPickup::flagTypeBlue_s = "blue";
    /*static*/ const std::string FlagPickup::flagTypeRed_s = "red";
    /*static*/ const std::string FlagPickup::flagTypeNeutral_s = "neutral";

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
        this->flagType_ = pickupFlagType::0;

        this->addTarget(ClassIdentifier<Pawn>::getIdentifier());
    }

    /**
    @brief
        Initializes the PickupIdentifier of this pickup.
    */
    void FlagPickup::initializeIdentifier(void)
    {
        std::string val1 = this->getFlagType();
        std::string type1 = "flagType";
        this->pickupIdentifier_->addParameter(type1, val1);
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
    const std::string& FlagPickup::getFlagType(void) const
    {
        switch(this->getFlagTypeDirect())
        {
            case pickupFlagType::blue:
                return FlagPickup::flagTypeBlue_s;
            case pickupFlagType::red:
                return FlagPickup::flagTypeRed_s;
            case pickupFlagType::neutral:
                return FlagPickup::flagTypeNeutral_s;
            default:
                orxout(internal_error, context::pickups) << "Invalid flagType in FlagPickup." << endl;
                return BLANKSTRING;
        }
    }

    /**
    @brief
        Set the type of the HealthPickup.
    @param type
        The type as a string.
    */
    void FlagPickup::setFlagType(std::string type)
    {
        if(type == FlagPickup::flagTypeRed_s)
            this->setFlagTypeDirect(pickupFlagType::2);
        else if(type == FlagPickup::flagTypeBlue_s)
            this->setFlagTypeDirect(pickupFlagType::1);
        else if(type == FlagPickup::flagTypeNeutral_s)
            this->setFlagTypeDirect(pickupFlagType::0);
        else
            orxout(internal_error, context::pickups) << "Invalid flagType '" << type << "' in FlagPickup." << endl;
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
               int team = getTeam(pawn->getPlayer);

               if(this->flagType_ == team){
            	   if(pawn->hasFlag_){
               		   teamScore_ = TeamScore_ + 1000;
                	   pawn->hasFlag_ = false;
                   }
               this->Pickupable::destroy();
               }else{
            	   pawn->hasFlag_ = true;
               }
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

    int FlagPickup::getTeam(PlayerInfo* player)
    {
        std::map<PlayerInfo*, int>::const_iterator it_player = this->teamnumbers_.find(player);
        if (it_player != this->teamnumbers_.end())
            return it_player->second;
        else
            return -1;
    }

}

	void FlagPickup::tick(float dt)
	   {
		   SUPER(FlagPickup, tick, dt);

	       Pawn* pawn = this->carrierToPawnHelper();

		   if(pawn->isAlive() && pawn->hasFlag_){
               this->Pickupable::destroy();
		   }

	   }
