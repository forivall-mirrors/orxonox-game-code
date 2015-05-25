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
 *      Oliver Scheuss
 *   Co-authors:
 *      ...
 *
 */

#include "NetworkFunction.h"
#include "NetworkFunctionManager.h"

namespace orxonox
{
    NetworkFunctionBase::NetworkFunctionBase(const std::string& name, const NetworkFunctionPointer& pointer)
    {
        static uint32_t networkID = 0;
        this->networkID_ = networkID++;
        this->name_ = name;
        this->pointer_ = pointer;
    }

    void NetworkFunctionBase::setNetworkID(uint32_t id)
    {
        NetworkFunctionManager::getInstance().unregisterFunction(this); // unregister with old id
        this->networkID_ = id;
        NetworkFunctionManager::getInstance().registerFunction(this);   // register with new id
    }
}
