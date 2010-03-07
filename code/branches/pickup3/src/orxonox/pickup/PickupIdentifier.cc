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

#include "PickupIdentifier.h"

#include "core/CoreIncludes.h"
#include "interfaces/Pickupable.h"

namespace orxonox
{
    
    /**
    @brief
        Constructor. Registers the object and initializes member variables.
    */
    PickupIdentifier::PickupIdentifier(Pickupable* pickup)
    {
        RegisterRootObject(PickupIdentifier);
        
        this->pickup_ = pickup;
    }
    
    PickupIdentifier::~PickupIdentifier()
    {
        
    }
    
    /**
    @brief
        Compares two PickupIdentifiers and returns 0 if a == b, <0 if a < b and >0 if a > b for a.compare(b).
    @param identifier
        Pointer to the second PickupIdentifier, b.
    @return
        Returns an integer. 0 if the two compared PickupIdentifiers are the same, <0 if a < b and >0 if a > b.
    */
    int PickupIdentifier::compare(const PickupIdentifier* identifier) const
    {
        //! If the classIdentifiers are not the same (meaning the PickupIdentifiers identify different classes), the obviously the two Pickupables identified by the PickupIdentifiers cannot be the same. An ordering is established through the alphabetical ordering of the respective classnames.
        if(!identifier->pickup_->getIdentifier()->isExactlyA(this->pickup_->getIdentifier()))
            return this->pickup_->getIdentifier()->getName().compare(identifier->pickup_->getIdentifier()->getName());
        
        //! If the class is the same for both PickupIdentifiers we go on to check the parameters of the class.
        //! If the two have a different number of parameters then obviusly something is very wrong.
        if(!(this->parameters_.size() == identifier->parameters_.size()))
        {
            COUT(1) << "Something went wrong in PickupIdentifier!" << std::endl;
            return this->parameters_.size()-identifier->parameters_.size();
        }
        
        //! We iterate through all parameters and compar their values (which are strings). The first parameter is the most significant. The ordering is once again established by the alphabetical comparison of the two value strings.
        for(std::map<std::string, std::string>::const_iterator it = this->parameters_.begin(); it != this->parameters_.end(); it++)
        {
            //!< If a parameter present in one of the identifiers is not found in the other, once again, something is very wrong.
            if(identifier->parameters_.find(it->first) == identifier->parameters_.end())
            {
                COUT(1) << "Something went wrong in PickupIdentifier!" << std::endl;
                return -1;
            }
            if(identifier->parameters_.find(it->first)->second != it->second)
                return it->second.compare(identifier->parameters_.find(it->first)->second);
        }
            
        return false;
    }
    
    /**
    @brief
        Add a parameter to the PickupIdentifier.
    @param name
        The name of the parameter.
    @param value
        The value of the parameter.
    @return
        Returns false if the parameter already existed, true if not.
    */
    bool PickupIdentifier::addParameter(std::string & name, std::string & value)
    {
        COUT(4) << "PickupIdentifier " << name << ", " << value << std::endl;
        
        if(!(this->parameters_.find(name) == this->parameters_.end()))
        {
            COUT(2) << "Request for adding a parameter that already exists for the PickupIdentififer was denied. name: '" << name << "', value: '" << value << "'."<<  std::endl;
            return false;
        }
        
        this->parameters_[name] = value;
        
        return true;
    }
    
}
