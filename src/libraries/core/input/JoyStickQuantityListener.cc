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
 *      Reto Grieder
 *   Co-authors:
 *      ...
 *
 */

#include "JoyStickQuantityListener.h"

#include "core/CoreIncludes.h"
#include "core/object/ObjectList.h"

namespace orxonox
{
    std::vector<JoyStick*> JoyStickQuantityListener::joyStickList_s;

    RegisterAbstractClass(JoyStickQuantityListener).inheritsFrom<Listable>();

    JoyStickQuantityListener::JoyStickQuantityListener()
    {
        RegisterObject(JoyStickQuantityListener);
    }

    //! Calls all registered objects and sets the static variable
    /*static*/ void JoyStickQuantityListener::changeJoyStickQuantity(const std::vector<JoyStick*>& joyStickList)
    {
        joyStickList_s = joyStickList;
        for (ObjectList<JoyStickQuantityListener>::iterator it = ObjectList<JoyStickQuantityListener>::begin(); it; ++it)
            it->JoyStickQuantityChanged(joyStickList);
    }
}
