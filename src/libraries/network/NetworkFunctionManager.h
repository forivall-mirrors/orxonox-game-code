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

#ifndef __NetworkFunctionManager_H__
#define __NetworkFunctionManager_H__

#include "NetworkPrereqs.h"

#include <cassert>
#include <map>

namespace orxonox
{
    class _NetworkExport NetworkFunctionManager
    {
        public:
            static NetworkFunctionManager& getInstance();

            static inline bool  isStatic( uint32_t networkID )  { return isStaticMap_[networkID]; }
            static void setNetworkID(const std::string& name, uint32_t id);
            static void destroyAllNetworkFunctions();
            static std::map<std::string, NetworkFunctionBase*>& getNameMap();

          private:
            static std::map<uint32_t, bool> isStaticMap_;
    };
}

#endif /* __NetworkFunctionManager_H__ */
