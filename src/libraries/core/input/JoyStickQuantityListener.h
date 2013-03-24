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

/**
@file
@brief
*/

#ifndef _JoyStickQuantityListener_H__
#define _JoyStickQuantityListener_H__

#include "InputPrereqs.h"

#include <vector>
#include "core/class/OrxonoxClass.h"

namespace orxonox
{
    //! Derive from this class to get informed when joy sticks get added/removed
    class _CoreExport JoyStickQuantityListener : virtual public OrxonoxClass
    {
        friend class InputManager;
    protected:
        JoyStickQuantityListener();
        virtual ~JoyStickQuantityListener() { }

        //! Returns a list with all JoySticks currently loaded
        const std::vector<JoyStick*>& getJoyStickList() const { return joyStickList_s; }

    private:
        //! Called whenever joy sticks get added/removed
        virtual void JoyStickQuantityChanged(const std::vector<JoyStick*>& joyStickList) = 0;

        static void changeJoyStickQuantity(const std::vector<JoyStick*>& joyStickList);

        //! Static variable that holds the latest distributed information
        static std::vector<JoyStick*> joyStickList_s;
    };
}

#endif /* _JoyStickQuantityListener_H__ */
