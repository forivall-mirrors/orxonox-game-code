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

#ifndef _PickupIdentifier_H__
#define _PickupIdentifier_H__

#include "OrxonoxPrereqs.h"

#include "core/OrxonoxClass.h" 
#include "core/Identifier.h"
#include <map>
#include <string>

namespace orxonox
{
    
    class _OrxonoxExport PickupIdentifier : virtual public OrxonoxClass
    {
        
        public:
            PickupIdentifier(void);
            ~PickupIdentifier();
            
            virtual int compare(const PickupIdentifier& identifier) const;
            
            void addClass(Identifier* identifier);
            bool addParameter(std::string & name, std::string & value);
            
        private:
            Identifier* classIdentifier_;
            std::map<std::string, std::string> parameters_;
            
            
    };
    
    //TODO: Needed?
    struct PickupIdentifierCompare
    {
        bool operator() (const PickupIdentifier& lhs, const PickupIdentifier& rhs) const
            { return lhs.compare(rhs) < 0; }
    };
    
    struct PickupIdentifierPtrCompare
    {
        bool operator() (const PickupIdentifier* lhs, const PickupIdentifier* rhs) const
            { return lhs->compare(*rhs) < 0; }
    };
    
}

#endif // _PickupIdentifier_H__
