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

#ifndef _MultiTriggerContainer_H__
#define _MultiTriggerContainer_H__

#include "objects/ObjectsPrereqs.h"

#include "core/BaseObject.h"

namespace orxonox
{

    class _ObjectsExport MultiTriggerContainer : public BaseObject
    {
        public:
            
            MultiTriggerContainer(BaseObject* creator);
            MultiTriggerContainer(BaseObject* creator, MultiTrigger* originator, BaseObject* data);
            ~MultiTriggerContainer();
            
            inline MultiTrigger* getOriginator(void)
                { return this->originator_; }
            inline BaseObject* getData(void)
                { return this->data_; }
            
        private:
            MultiTrigger* originator_;
            BaseObject* data_;
    };
    
}

#endif // _MultiTriggerContainer_H__
