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

    ModuleInstance::ModuleInstance(const std::string& name)
        : name_(name)
        , dynLib_(NULL)
    {
    }

    ModuleInstance::~ModuleInstance()
    {
        this->deleteAllStaticallyInitializedInstances();
    }

    void ModuleInstance::addStaticallyInitializedInstance(StaticallyInitializedInstance* instance)
    {
        this->staticallyInitializedInstances_.insert(instance);
    }

    void ModuleInstance::loadAllStaticallyInitializedInstances()
    {
        for (std::set<StaticallyInitializedInstance*>::iterator it = this->staticallyInitializedInstances_.begin(); it != this->staticallyInitializedInstances_.end(); ++it)
            (*it)->load();
    }

    void ModuleInstance::unloadAllStaticallyInitializedInstances()
    {
        for (std::set<StaticallyInitializedInstance*>::iterator it = this->staticallyInitializedInstances_.begin(); it != this->staticallyInitializedInstances_.end(); ++it)
            (*it)->unload();
    }

    void ModuleInstance::removeStaticallyInitializedInstance(StaticallyInitializedInstance* instance)
    {
        this->staticallyInitializedInstances_.erase(instance);
    }

    void ModuleInstance::deleteAllStaticallyInitializedInstances()
    {
        std::set<StaticallyInitializedInstance*> copy(this->staticallyInitializedInstances_);
        this->staticallyInitializedInstances_.clear();
        for (std::set<StaticallyInitializedInstance*>::iterator it = copy.begin(); it != copy.end(); ++it)
            delete (*it);
    }


    /*static*/ void ModuleInstance::setCurrentModuleInstance(ModuleInstance* instance)
    {
        ModuleInstance::currentModuleInstance_s = instance;
    }

    /*static*/ ModuleInstance* ModuleInstance::getCurrentModuleInstance()
    {
        if (!ModuleInstance::currentModuleInstance_s)
            ModuleInstance::currentModuleInstance_s = new ModuleInstance("");
        return ModuleInstance::currentModuleInstance_s;
    }
}
