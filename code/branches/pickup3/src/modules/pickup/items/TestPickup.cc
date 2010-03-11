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

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "interfaces/PickupCarrier.h"
#include "worldentities/pawns/Pawn.h"
#include "pickup/PickupIdentifier.h"

#include "TestPickup.h"

namespace orxonox {
 
    CreateFactory(TestPickup);
    
    /*static*/ const std::string TestPickup::testTypeNone_s = "none";
    /*static*/ const std::string TestPickup::testTypeUse_s = "use";
    /*static*/ const std::string TestPickup::testTypeDrop_s = "drop";
    
    TestPickup::TestPickup(BaseObject* creator) : Pickup(creator)
    {
        RegisterObject(TestPickup);
        
        this->addTarget(ClassIdentifier<Pawn>::getIdentifier());
        this->setActivationTypeDirect(pickupActivationType::immediate);
        this->setDurationTypeDirect(pickupDurationType::once);
        this->testType_ = pickupTestType::none;
    }
    
    TestPickup::~TestPickup()
    {
        
    }
    
    void TestPickup::initializeIdentifier(void)
    {
        std::string val = this->getTestType();
        std::string type = "testType";
        this->pickupIdentifier_->addParameter(type, val);
    }
    
    void TestPickup::XMLPort(Element& xmlelement, orxonox::XMLPort::Mode mode)
    {
        SUPER(TestPickup, XMLPort, xmlelement, mode);
        
        XMLPortParam(TestPickup, "testType", setTestType, getTestType, xmlelement, mode);
        
        this->initializeIdentifier();
    }
    
    void TestPickup::changedUsed(void)
    {
        SUPER(TestPickup, changedUsed);
        
        if(this->isUsed())
        {
            PickupCarrier* carrier = this->getCarrier();
            if(this->getTestTypeDirect() != pickupTestType::none && carrier != NULL)
            {
                std::set<Pickupable*> pickups = carrier->getPickups();
                for(std::set<Pickupable*>::iterator it = pickups.begin(); it != pickups.end(); it++)
                {
                    Pickup* pickup = dynamic_cast<Pickup*>(*it);
                    if(this->getTestTypeDirect() == pickupTestType::use)
                    {
                        if(pickup != NULL && pickup != this && pickup->isOnUse() && !pickup->isUsed())
                        {
                            pickup->setUsed(true);
                        }
                    }
                    if(this->getTestTypeDirect() == pickupTestType::drop)
                    {
                        if(pickup != NULL && pickup != this)
                        {
                            carrier->drop(pickup);
                        }
                    }
                }
            }
            this->destroy();
        }
    }
    
    const std::string& TestPickup::getTestType(void)
    {
        switch(this->getTestTypeDirect())
        {
            case pickupTestType::none:
                return TestPickup::testTypeNone_s;
            case pickupTestType::use:
                return TestPickup::testTypeUse_s;
            case pickupTestType::drop:
                return TestPickup::testTypeDrop_s;
            default:
                return BLANKSTRING;
        }
    }
    
    void TestPickup::setTestType(const std::string& type)
    {
        if(type == TestPickup::testTypeNone_s)
        {
            this->setTestTypeDirect(pickupTestType::none);
        }
        else if(type == TestPickup::testTypeUse_s)
        {
            this->setTestTypeDirect(pickupTestType::use);
        }
        else if(type == TestPickup::testTypeDrop_s)
        {
            this->setTestTypeDirect(pickupTestType::drop);
        }
    }
    
    void TestPickup::clone(OrxonoxClass*& item)
    {
        if(item == NULL)
            item = new TestPickup(this);
        
        SUPER(TestPickup, clone, item);
        
        TestPickup* pickup = dynamic_cast<TestPickup*>(item);
        pickup->setTestTypeDirect(this->getTestTypeDirect());
        
        pickup->initializeIdentifier();
    }
    
}
