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
    NetworkFunctionBase::NetworkFunctionBase(const std::string& name, const NetworkFunctionPointer& p)
    {
        static uint32_t networkID = 0;
        this->networkID_ = networkID++;

        this->name_ = name;
        NetworkFunctionManager::getInstance().getNameMap()[name] = this;
        NetworkFunctionManager::getInstance().getFunctorMap()[p] = this;
        NetworkFunctionManager::getInstance().getIdMap()[this->getNetworkID()] = this;
    }

    void NetworkFunctionBase::setNetworkID(uint32_t id)
    {
        NetworkFunctionManager::getInstance().getIdMap().erase(this->networkID_);  // remove old id
        this->networkID_ = id;
        NetworkFunctionManager::getInstance().getIdMap()[this->networkID_] = this; // add new id
    }
}
