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
 *      Fabian 'x3n' Landau
 *   Co-authors:
 *      ...
 *
 */

#include "NetworkFunctionManager.h"
#include "NetworkFunction.h"

namespace orxonox
{
    /* static */NetworkFunctionManager& NetworkFunctionManager::getInstance()
    {
        static NetworkFunctionManager instance;
        return instance;
    }

    void NetworkFunctionManager::registerFunction(NetworkFunctionBase* function)
    {
        this->functions_.insert(function);
        this->nameMap_[function->getName()] = function;
        this->idMap_[function->getNetworkID()] = function;
        this->functorMap_[function->getPointer()] = function;
    }

    void NetworkFunctionManager::unregisterFunction(NetworkFunctionBase* function)
    {
        this->functions_.erase(function);
        this->nameMap_.erase(function->getName());
        this->idMap_.erase(function->getNetworkID());
        this->functorMap_.erase(function->getPointer());
    }

    void NetworkFunctionManager::destroyAllNetworkFunctions()
    {
        std::set<NetworkFunctionBase*>::iterator it;
        for (it = this->functions_.begin(); it != this->functions_.end(); ++it)
            delete (*it);
    }

    NetworkFunctionBase* NetworkFunctionManager::getFunctionByName(const std::string& name)
    {
        std::map<std::string, NetworkFunctionBase*>::iterator it = nameMap_.find(name);
        assert(it != nameMap_.end());
        return it->second;
    }

    NetworkFunctionBase* NetworkFunctionManager::getFunctionByFunctionPointer(const NetworkFunctionPointer& p)
    {
        std::map<NetworkFunctionPointer, NetworkFunctionBase*>::iterator it = functorMap_.find(p);
        assert(it != functorMap_.end());
        return it->second;
    }

    NetworkFunctionBase* NetworkFunctionManager::getFunctionByNetworkId(uint32_t id)
    {
        std::map<uint32_t, NetworkFunctionBase*>::iterator it = idMap_.find(id);
        assert(it != idMap_.end());
        return it->second;
    }
}
