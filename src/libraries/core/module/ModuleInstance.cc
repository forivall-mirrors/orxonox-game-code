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

#include "ModuleInstance.h"

#include "StaticallyInitializedInstance.h"

namespace orxonox
{
    ModuleInstance* ModuleInstance::currentModuleInstance_s = NULL;

    ModuleInstance::~ModuleInstance()
    {
        std::list<StaticallyInitializedInstance*> copy(this->staticallyInitializedInstances_);
        for (std::list<StaticallyInitializedInstance*>::iterator it = this->staticallyInitializedInstances_.begin(); it != this->staticallyInitializedInstances_.end(); ++it)
            delete (*it);
    }

    void ModuleInstance::addStaticallyInitializedInstance(StaticallyInitializedInstance* instance)
    {
        this->staticallyInitializedInstances_.push_back(instance);
    }

    void ModuleInstance::loadAllStaticallyInitializedInstances()
    {
        for (std::list<StaticallyInitializedInstance*>::iterator it = this->staticallyInitializedInstances_.begin(); it != this->staticallyInitializedInstances_.end(); ++it)
            (*it)->load();
    }

    void ModuleInstance::removeStaticallyInitializedInstance(StaticallyInitializedInstance* instance)
    {
        this->staticallyInitializedInstances_.remove(instance);
    }

    /*static*/ void ModuleInstance::setCurrentModuleInstance(ModuleInstance* instance)
    {
        ModuleInstance::currentModuleInstance_s = instance;
    }

    /*static*/ ModuleInstance* ModuleInstance::getCurrentModuleInstance()
    {
        if (!ModuleInstance::currentModuleInstance_s)
            ModuleInstance::currentModuleInstance_s = new ModuleInstance();
        return ModuleInstance::currentModuleInstance_s;
    }
}
