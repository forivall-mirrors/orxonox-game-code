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

#ifndef _StaticInitializationManager_H__
#define _StaticInitializationManager_H__

#include "core/CorePrereqs.h"

#include <list>

namespace orxonox
{
    class _CoreExport StaticInitializationManager
    {
        friend class Core;

        public:
            static StaticInitializationManager& getInstance();

            StaticInitializationManager() {}
            virtual ~StaticInitializationManager() {}

            void addHandler(StaticInitializationHandler* handler);
            void removeHandler(StaticInitializationHandler* handler);

            void loadModule(ModuleInstance* module);
            void unloadModule(ModuleInstance* module);

        private:
            std::list<StaticInitializationHandler*> handlers_;
    };
}

#endif /* _StaticInitializationManager_H__ */
