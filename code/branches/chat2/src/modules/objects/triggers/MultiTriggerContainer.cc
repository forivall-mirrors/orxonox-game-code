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
 *      ...
 *   Co-authors:
 *      ...
 *
*/

#include "MultiTriggerContainer.h"

#include "core/CoreIncludes.h"

namespace orxonox
{

    CreateFactory(MultiTriggerContainer);
    
    MultiTriggerContainer::MultiTriggerContainer(BaseObject* creator) : BaseObject(creator), originator_(NULL), data_(NULL)
    {
        RegisterObject(MultiTriggerContainer);
        
    }
    
    MultiTriggerContainer::MultiTriggerContainer(BaseObject* creator, MultiTrigger* originator, BaseObject* data) : BaseObject(creator), originator_(originator), data_(data)
    {
        RegisterObject(MultiTriggerContainer);
        
    }
    
    MultiTriggerContainer::~MultiTriggerContainer()
    {
        
    }


}
