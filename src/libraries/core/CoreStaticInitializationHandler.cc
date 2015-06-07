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

#include "CoreIncludes.h"
#include "module/ModuleInstance.h"
#include "object/Iterator.h"
#include "class/IdentifierManager.h"
#include "singleton/ScopeManager.h"
#include "command/ConsoleCommandManager.h"
#include "commandline/CommandLineParser.h"

namespace orxonox
{
    void CoreStaticInitializationHandler::setupHandler()
    {
        // initialize singletons
        new IdentifierManager();
        new ScopeManager();
        new CommandLineParser();
        new ConsoleCommandManager();
    }

    void CoreStaticInitializationHandler::shutdownHandler()
    {
        delete &ConsoleCommandManager::getInstance();
        delete &CommandLineParser::getInstance();
        delete &ScopeManager::getInstance();
        delete &IdentifierManager::getInstance();
    }

    void CoreStaticInitializationHandler::loadModule(ModuleInstance* module)
    {
        this->loadInstances(module);
        if (this->bInitInstances_)
            this->initInstances(module);
    }

    void CoreStaticInitializationHandler::loadInstances(ModuleInstance* module)
    {
        // the order of initialization is important: handlers > identifiers > singletons > everything else
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

        // until now every object (including singletons) of the unloaded identifiers should have been destroyed in a controlled manner.
        // every remaining object is now destroyed in random order.
        this->destroyObjects(module);

        // all objects are gone now and we can unload identifiers
        module->unloadAllStaticallyInitializedInstances(StaticInitialization::IDENTIFIER);
        module->unloadAllStaticallyInitializedInstances(StaticInitialization::STATIC_INITIALIZATION_HANDLER);
    }

    void CoreStaticInitializationHandler::destroyObjects(ModuleInstance* module)
    {
        // collect all identifiers that are about to be unloaded
        std::set<Identifier*> identifiers;
        const std::set<StaticallyInitializedInstance*>& instances = module->getInstances(StaticInitialization::IDENTIFIER);
        for (std::set<StaticallyInitializedInstance*>::const_iterator it = instances.begin(); it != instances.end(); ++it)
            identifiers.insert(&static_cast<StaticallyInitializedIdentifier*>(*it)->getIdentifier());

        // destroy objects. some objects may survive this at first because they still have smart pointers pointing at them. this is
        // ok as long as those smart pointers are held by objects that are also about to be destroyed in the same loop. this means
        // that objects within one module may reference each other by smart pointers. but it is not allowed that objects from another
        // module (which is not unloaded) uses smart pointers to point at objects inside the unloaded module. this will lead to a crash.
        for (std::set<Identifier*>::iterator it = identifiers.begin(); it != identifiers.end(); ++it)
            (*it)->destroyObjects();

        // check if all objects were really destroyed. this is not the case if an object is referenced by a smart pointer from another
        // module (or if two objects inside this module reference each other). this will lead to a crash and must be fixed (e.g. by
        // changing object dependencies; or by changing the logic that allows modules to be unloaded).
        for (std::set<Identifier*>::iterator it = identifiers.begin(); it != identifiers.end(); ++it)
        {
            ObjectListBase* objectList = Context::getRootContext()->getObjectList(*it);
            if (objectList->size() > 0)
            {
                orxout(internal_error) << "There are still " << objectList->size() << " objects of type " << (*it)->getName()
                    << " after unloading the Identifier. This may lead to a crash" << endl;
            }
        }

        // destroy object-lists in all contexts
        for (std::set<Identifier*>::iterator it_identifier = identifiers.begin(); it_identifier != identifiers.end(); ++it_identifier)
        {
            // only do this if the Identifier is not a Context itself; otherwise we delete the list we're iterating over
            if (!(*it_identifier)->isExactlyA(Class(Context)))
            {
                // iterate over all contexts
                for (ObjectList<Context>::iterator it_context = ObjectList<Context>::begin(); it_context != ObjectList<Context>::end(); ++it_context)
                    it_context->destroyObjectList((*it_identifier));
            }
        }
    }
}
