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
    @file MetaPickup.cc
    @brief Implementation of the MetaPickup class.
*/

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

#include "interfaces/PickupCarrier.h"
#include "worldentities/pawns/Pawn.h"

#include "MetaPickup.h"

namespace orxonox {

    RegisterClass(MetaPickup);

    //! Setting the static variables to their values.
    /*static*/ const std::string MetaPickup::metaTypeNone_s = "none";
    /*static*/ const std::string MetaPickup::metaTypeUse_s = "use";
    /*static*/ const std::string MetaPickup::metaTypeDrop_s = "drop";
    /*static*/ const std::string MetaPickup::metaTypeDestroy_s = "destroy";
    /*static*/ const std::string MetaPickup::metaTypeDestroyCarrier_s = "destroyCarrier";

    /**
    @brief
        Constructor. Registers and initializes the object.
    */
    MetaPickup::MetaPickup(Context* context) : Pickup(context)
    {
        RegisterObject(MetaPickup);

        this->initialize();
    }

    /**
    @brief
        Destructor.
    */
    MetaPickup::~MetaPickup()
    {

    }

    /**
    @brief
        Initializes the object.
    */
    void MetaPickup::initialize(void)
    {
        this->addTarget(ClassIdentifier<PickupCarrier>::getIdentifier());

        this->setDurationType(pickupDurationType::once);
        this->metaType_ = pickupMetaType::none;
    }

    /**
    @brief
        Method for creating a MetaPickup object through XML.
    */
    void MetaPickup::XMLPort(Element& xmlelement, orxonox::XMLPort::Mode mode)
    {
        SUPER(MetaPickup, XMLPort, xmlelement, mode);

        XMLPortParam(MetaPickup, "metaType", setMetaTypeAsString, getMetaTypeAsString, xmlelement, mode);
    }

    /**
    @brief
        Is called when the pickup has transited from used to unused or the other way around.
        Any Class overwriting this method must call its SUPER function by adding SUPER(Classname, changedUsed); to their changdeUsed method.
    */
    void MetaPickup::changedUsed(void)
    {
        SUPER(MetaPickup, changedUsed);

        // If the MetaPickup transited to used, and the metaType is not none.
        if(this->isUsed() && this->metaType_ != pickupMetaType::none)
        {
            PickupCarrier* carrier = this->getCarrier();
            if(this->getMetaType() != pickupMetaType::none && carrier != NULL)
            {
                // If the metaType is destroyCarrier, then the PickupCarrier is destroyed.
                if(this->getMetaType() == pickupMetaType::destroyCarrier)
                {
                    Pawn* pawn = orxonox_cast<Pawn*>(carrier);
                    pawn->kill();
                    return;
                }
                std::set<Pickupable*> pickups = carrier->getPickups();
                // Iterate over all Pickupables of the PickupCarrier.
                for(std::set<Pickupable*>::iterator it = pickups.begin(); it != pickups.end(); it++)
                {
                    Pickupable* pickup = (*it);
                    if(pickup == NULL || pickup == this)
                        continue;

                    // If the metaType is use, then the Pickupable is set to used.
                    if(this->getMetaType() == pickupMetaType::use && !pickup->isUsed())
                    {
                        pickup->setUsed(true);
                    }
                    // If the metaType is drop, then the Pickupable is dropped.
                    else if(this->getMetaType() == pickupMetaType::drop)
                    {
                        pickup->drop();
                    }
                    // If the metaType is destroy, then the Pickupable is destroyed.
                    else if(this->getMetaType() == pickupMetaType::destroy)
                    {
                        pickup->Pickupable::destroy();
                    }
                }
            }
            this->Pickupable::destroy();
        }
    }

    /**
    @brief
        Get the meta type of this MetaPickup.
    @return
        Returns a string with the meta type of the MetaPickup.
    */
    const std::string& MetaPickup::getMetaTypeAsString(void) const
    {
        switch(this->getMetaType())
        {
            case pickupMetaType::none:
                return MetaPickup::metaTypeNone_s;
            case pickupMetaType::use:
                return MetaPickup::metaTypeUse_s;
            case pickupMetaType::drop:
                return MetaPickup::metaTypeDrop_s;
            case pickupMetaType::destroy:
                return MetaPickup::metaTypeDestroy_s;
            case pickupMetaType::destroyCarrier:
                return MetaPickup::metaTypeDestroyCarrier_s;
            default:
                return BLANKSTRING;
        }
    }

    /**
    @brief
        Set the meta type of this MetaPickup.
    @param type
        A string with the type to be set.
    */
    void MetaPickup::setMetaTypeAsString(const std::string& type)
    {
        if(type == MetaPickup::metaTypeNone_s)
        {
            this->setMetaType(pickupMetaType::none);
        }
        else if(type == MetaPickup::metaTypeUse_s)
        {
            this->setMetaType(pickupMetaType::use);
        }
        else if(type == MetaPickup::metaTypeDrop_s)
        {
            this->setMetaType(pickupMetaType::drop);
        }
        else if(type == MetaPickup::metaTypeDestroy_s)
        {
            this->setMetaType(pickupMetaType::destroy);
        }
        else if(type == MetaPickup::metaTypeDestroyCarrier_s)
        {
            this->setMetaType(pickupMetaType::destroyCarrier);
        }
        else
            orxout(internal_warning, context::pickups) << "Invalid metaType '" << type << "' in MetaPickup." << endl;
    }

}
