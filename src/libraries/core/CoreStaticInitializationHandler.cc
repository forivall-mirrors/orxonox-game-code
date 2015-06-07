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

#include "CoreStaticInitializationHandler.h"

#include "module/ModuleInstance.h"
#include "class/IdentifierManager.h"

namespace orxonox
{
    void CoreStaticInitializationHandler::setupHandler()
    {
        // TODO
    }

    void CoreStaticInitializationHandler::shutdownHandler()
    {
        // TODO
    }

    void CoreStaticInitializationHandler::loadModule(ModuleInstance* module)
    {
        this->loadInstances(module);
        if (this->bInitInstances_)
            this->initInstances(module);
    }

    void CoreStaticInitializationHandler::loadInstances(ModuleInstance* module)
    {
        // the order of initialization is important
        module->loadAllStaticallyInitializedInstances(StaticInitialization::STATIC_INITIALIZATION_HANDLER);
        module->loadAllStaticallyInitializedInstances(StaticInitialization::IDENTIFIER);
        module->loadAllStaticallyInitializedInstances(StaticInitialization::SCOPED_SINGLETON_WRAPPER);
        module->loadAllStaticallyInitializedInstances(StaticInitialization::COMMAND_LINE_ARGUMENT);
        module->loadAllStaticallyInitializedInstances(StaticInitialization::CONSOLE_COMMAND);
    }

    void CoreStaticInitializationHandler::initInstances(ModuleInstance* module)
    {
        IdentifierManager::getInstance().createClassHierarchy();
    }

    void CoreStaticInitializationHandler::unloadModule(ModuleInstance* module)
    {
        // inverted order of initialization
        module->unloadAllStaticallyInitializedInstances(StaticInitialization::CONSOLE_COMMAND);
        module->unloadAllStaticallyInitializedInstances(StaticInitialization::COMMAND_LINE_ARGUMENT);
        module->unloadAllStaticallyInitializedInstances(StaticInitialization::SCOPED_SINGLETON_WRAPPER);
        module->unloadAllStaticallyInitializedInstances(StaticInitialization::IDENTIFIER);
        module->unloadAllStaticallyInitializedInstances(StaticInitialization::STATIC_INITIALIZATION_HANDLER);
    }
}
