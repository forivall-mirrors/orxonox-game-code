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
 *      Fabian 'x3n' Landau
 *
 */

#include "NetworkFunctionIncludes.h"
#include "NetworkFunctionManager.h"

#include "core/module/StaticInitializationHandlerIncludes.h"
#include "NetworkStaticInitializationHandler.h"

namespace orxonox
{
    RegisterStaticInitializationHandler(NetworkStaticInitializationHandler);

    void StaticallyInitializedNetworkFunction::load()
    {
        NetworkFunctionManager::getInstance().registerFunction(this->function_);
    }

    void StaticallyInitializedNetworkFunction::unload()
    {
        NetworkFunctionManager::getInstance().unregisterFunction(this->function_);
    }

    uint32_t getNetworkIdForPointer(const NetworkFunctionPointer& pointer)
    {
        return NetworkFunctionManager::getInstance().getFunctionByFunctionPointer(pointer)->getNetworkID();
    }
}
