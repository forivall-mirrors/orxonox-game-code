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

namespace orxonox
{
    
    PickupIdentifier::PickupIdentifier()
    {
        RegisterRootObject(PickupIdentifier);
        
        
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
        Returns an int.
    */
    int PickupIdentifier::compare(const PickupIdentifier& identifier) const
    {
        if(!identifier.classIdentifier_->isExactlyA(this->classIdentifier_))
            return this->classIdentifier_->getName().compare(identifier.classIdentifier_->getName());
        
        if(!(this->parameters_.size() == identifier.parameters_.size()))
        {
            COUT(1) << "Something went wrong in PickupIdentifier!" << std::endl;
            return this->parameters_.size()-identifier.parameters_.size();
        }
        
        for(std::map<std::string, std::string>::const_iterator it = this->parameters_.begin(); it != this->parameters_.end(); it++)
        {
            if(identifier.parameters_.find(it->first) == identifier.parameters_.end())
            {
                COUT(1) << "Something went wrong in PickupIdentifier!" << std::endl;
                return -1;
            }
            if(identifier.parameters_.find(it->first)->second != it->second)
                return it->second.compare(identifier.parameters_.find(it->first)->second);
        }
            
        return false;
    }
    
    void PickupIdentifier::addClass(Identifier* identifier)
    {
        this->classIdentifier_ = identifier;
    }
    
    bool PickupIdentifier::addParameter(std::string & name, std::string & value)
    {
        if(!(this->parameters_.find(name) == this->parameters_.end()))
        {
            COUT(2) << "Request for adding a parameter that already exists for the PickupIdentififer was denied." << std::endl;
            return false;
        }
        
        this->parameters_[name] = value;
        
        return true;
    }
    
}
