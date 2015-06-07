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

#ifndef _ModuleInstance_H__
#define _ModuleInstance_H__

#include "core/CorePrereqs.h"

#include <set>
#include <map>
#include <string>

#include "StaticallyInitializedInstance.h"

namespace orxonox
{
    class _CoreExport ModuleInstance
    {
        public:
            ModuleInstance(const std::string& name);
            ~ModuleInstance();

            void addStaticallyInitializedInstance(StaticallyInitializedInstance* instance);
            void removeStaticallyInitializedInstance(StaticallyInitializedInstance* instance);

            void loadAllStaticallyInitializedInstances(StaticInitialization::Type type);
            void unloadAllStaticallyInitializedInstances(StaticInitialization::Type type);

            inline const std::set<StaticallyInitializedInstance*>& getInstances(StaticInitialization::Type type)
                { return this->staticallyInitializedInstancesByType_[type]; }

            void deleteAllStaticallyInitializedInstances();

            inline const std::string& getName() const
                { return this->name_; }

            inline void setDynLib(DynLib* dynLib)
                { this->dynLib_ = dynLib; }
            inline DynLib* getDynLib() const
                { return this->dynLib_; }

            static void setCurrentModuleInstance(ModuleInstance* instance);
            static ModuleInstance* getCurrentModuleInstance();

        private:
            std::map<StaticInitialization::Type, std::set<StaticallyInitializedInstance*> > staticallyInitializedInstancesByType_;
            std::string name_;
            DynLib* dynLib_;

            static ModuleInstance* currentModuleInstance_s;
    };
}

#endif /* _ModuleInstance_H__ */
