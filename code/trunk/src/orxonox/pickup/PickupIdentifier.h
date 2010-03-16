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
    @file PickupIdentifier.h
    @brief Definition of the PickupIdentifier class.
*/

#ifndef _PickupIdentifier_H__
#define _PickupIdentifier_H__

#include "OrxonoxPrereqs.h"

#include <map>
#include <string>
#include "core/Identifier.h"

#include "core/OrxonoxClass.h" 

namespace orxonox
{
    
    /**
    @brief
        The purpose of the PickupIdentifier class is to identify different types of pickups allthough they are of the same class.
        This allows for more generic classes (or pickups in this case) that can be a number of different pickup types and can be identified as such without the need for a lot of different classes. An example is the HealthPickup class that encompasses a wide variety of different types of health pickups, e.g a HealthPickup that adds 10 health every second for 10 seconds or a HealthPickup that adds 100 health as soon as it is picked up, a.s.o.
        To that purpose this class provides functionality to compare two PickupIdentifier (and since all Pickupables have an Identifier through that Pickupables can be compared). It als provides functionality to add parameters that distinguish between different types of pickups in the same pickup class.
        Lastly a struct is provided that can be used in stl containers to establish a strictly lesser ordering between PickupIdentifiers (and thus Pickupables).
    @author
        Damian 'Mozork' Frick
    */
    class _OrxonoxExport PickupIdentifier : virtual public OrxonoxClass
    {
        
        public:
            PickupIdentifier(Pickupable* pickup); //!< Constructor.
            ~PickupIdentifier(); //!< Destructor.
            
            virtual int compare(const PickupIdentifier* identifier) const; //!< Compares two PickupIdentifiers and returns 0 if a == b, <0 if a < b and >0 if a > b for a.compare(b).
            
            bool addParameter(std::string & name, std::string & value); //!< Add a parameter to the PickupIdentifier.
            
        private:
            Pickupable* pickup_; //!< The Pickupable the PickupIdentififer is for.
            std::map<std::string, std::string> parameters_; //!< The parameters identifying the type of the pickup beyond the class.
            
    };
    
    /**
    @brief
        Struct that overloads the compare operation between two PickupIdentifier pointers.
    */
    struct PickupIdentifierCompare
    {
        bool operator() (const PickupIdentifier* lhs, const PickupIdentifier* rhs) const
            { return lhs->compare(rhs) < 0; }
    };
    
}

#endif // _PickupIdentifier_H__
