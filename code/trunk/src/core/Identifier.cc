/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
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

/**
    @file Identifier.cc
    @brief Implementation of the Identifier class.
*/

#include <ostream>

#include "Identifier.h"
#include "Factory.h"
#include "Executor.h"
#include "CommandExecutor.h"

namespace orxonox
{
    // ###############################
    // ###       Identifier        ###
    // ###############################
    int Identifier::hierarchyCreatingCounter_s = 0; // Set the static member variable hierarchyCreatingCounter_s to zero (this static member variable is ok: it's used in main(), not before)

    /**
        @brief Constructor: No factory, no object created, new ObjectList and a unique networkID.
    */
    Identifier::Identifier()
    {
        this->bCreatedOneObject_ = false;
        this->factory_ = 0;

        this->bHasConfigValues_ = false;
        this->bHasConsoleCommands_ = false;

        this->children_ = new std::set<const Identifier*>();
        this->directChildren_ = new std::set<const Identifier*>();

        // Use a static variable because the classID gets created before main() and that's why we should avoid static member variables
        static unsigned int classIDcounter_s = 0;
        this->classID_ = classIDcounter_s++;
    }

    /**
        @brief Destructor: Deletes the list containing the children.
    */
    Identifier::~Identifier()
    {
        delete this->children_;
        delete this->directChildren_;
    }

    /**
        @brief Initializes the Identifier with a list containing all parents of the class the Identifier belongs to.
        @param parents A list containing all parents
    */
    void Identifier::initialize(std::set<const Identifier*>* parents)
    {
        COUT(4) << "*** Identifier: Initialize " << this->name_ << "-Singleton." << std::endl;
        this->bCreatedOneObject_ = true;

        if (parents)
        {
            this->parents_ = (*parents);
            this->directParents_ = (*parents);

            // Iterate through all parents
            for (std::set<const Identifier*>::iterator it = parents->begin(); it != parents->end(); ++it)
            {
                // Tell the parent we're one of it's children
                (*it)->getChildrenIntern().insert((*it)->getChildrenIntern().end(), this);

                // Erase all parents of our parent from our direct-parent-list
                for (std::set<const Identifier*>::const_iterator it1 = (*it)->getParents().begin(); it1 != (*it)->getParents().end(); ++it1)
                {
                    // Search for the parent's parent in our direct-parent-list
                    for (std::set<const Identifier*>::iterator it2 = this->directParents_.begin(); it2 != this->directParents_.end(); ++it2)
                    {
                        if ((*it1) == (*it2))
                        {
                            // We've found a non-direct parent in our list: Erase it
                            this->directParents_.erase(it2);
                            break;
                        }
                    }
                }
            }

            // Now iterate through all direct parents
            for (std::set<const Identifier*>::iterator it = this->directParents_.begin(); it != this->directParents_.end(); ++it)
            {
                // Tell the parent we're one of it's direct children
                (*it)->getDirectChildrenIntern().insert((*it)->getDirectChildrenIntern().end(), this);
            }
        }
    }

    /**
        @brief Creates an object of the type the Identifier belongs to.
        @return The new object
    */
    BaseObject* Identifier::fabricate()
    {
        if (this->factory_)
        {
            return this->factory_->fabricate(); // We have to return a BaseObject, because we don't know the exact type.
        }
        else
        {
            COUT(1) << "An error occurred in Identifier.cc:" << std::endl;
            COUT(1) << "Error: Cannot fabricate an object of type '" << this->name_ << "'. Class has no factory." << std::endl;
            COUT(1) << "Aborting..." << std::endl;
            abort();
            return NULL;
        }
    }

    /**
        @brief Sets the network ID to a new value and changes the entry in the Factory.
        @param id The new network ID
    */
    void Identifier::setNetworkID(unsigned int id)
    {
        Factory::changeNetworkID(this, this->classID_, id);
        this->classID_ = id;
    }

    /**
        @brief Returns true, if the Identifier is at least of the given type.
        @param identifier The identifier to compare with
    */
    bool Identifier::isA(const Identifier* identifier) const
    {
        return (identifier == this || (this->parents_.find(identifier) != this->children_->end()));
    }

    /**
        @brief Returns true, if the Identifier is exactly of the given type.
        @param identifier The identifier to compare with
    */
    bool Identifier::isExactlyA(const Identifier* identifier) const
    {
        return (identifier == this);
    }

    /**
        @brief Returns true, if the assigned identifier is a child of the given identifier.
        @param identifier The identifier to compare with
    */
    bool Identifier::isChildOf(const Identifier* identifier) const
    {
        return (this->parents_.find(identifier) != this->children_->end());
    }

    /**
        @brief Returns true, if the assigned identifier is a direct child of the given identifier.
        @param identifier The identifier to compare with
    */
    bool Identifier::isDirectChildOf(const Identifier* identifier) const
    {
        return (this->directParents_.find(identifier) != this->children_->end());
    }

    /**
        @brief Returns true, if the assigned identifier is a parent of the given identifier.
        @param identifier The identifier to compare with
    */
    bool Identifier::isParentOf(const Identifier* identifier) const
    {
        return (this->children_->find(identifier) != this->children_->end());
    }

    /**
        @brief Returns true, if the assigned identifier is a direct parent of the given identifier.
        @param identifier The identifier to compare with
    */
    bool Identifier::isDirectParentOf(const Identifier* identifier) const
    {
        return (this->directChildren_->find(identifier) != this->children_->end());
    }

    /**
        @brief Returns the map that stores all Identifiers.
        @return The map
    */
    std::map<std::string, Identifier*>& Identifier::getIdentifierMapIntern()
    {
        static std::map<std::string, Identifier*> identifierMap;
        return identifierMap;
    }

    /**
        @brief Returns the map that stores all Identifiers.
        @return The map
    */
    std::map<std::string, Identifier*>& Identifier::getLowercaseIdentifierMapIntern()
    {
        static std::map<std::string, Identifier*> lowercaseIdentifierMap;
        return lowercaseIdentifierMap;
    }

    /**
        @brief Adds the ConfigValueContainer of a variable, given by the string of its name.
        @param varname The name of the variablee
        @param container The container
    */
    void Identifier::addConfigValueContainer(const std::string& varname, ConfigValueContainer* container)
    {
        this->bHasConfigValues_ = true;
        this->configValues_[varname] = container;
        this->configValues_LC_[getLowercase(varname)] = container;
    }

    /**
        @brief Returns the ConfigValueContainer of a variable, given by the string of its name.
        @param varname The name of the variable
        @return The ConfigValueContainer
    */
    ConfigValueContainer* Identifier::getConfigValueContainer(const std::string& varname)
    {
        std::map<std::string, ConfigValueContainer*>::const_iterator it = configValues_.find(varname);
        if (it != configValues_.end())
            return ((*it).second);
        else
            return 0;
    }

    /**
        @brief Returns the ConfigValueContainer of a variable, given by the string of its name in lowercase.
        @param varname The name of the variable in lowercase
        @return The ConfigValueContainer
    */
    ConfigValueContainer* Identifier::getLowercaseConfigValueContainer(const std::string& varname)
    {
        std::map<std::string, ConfigValueContainer*>::const_iterator it = configValues_LC_.find(varname);
        if (it != configValues_LC_.end())
            return ((*it).second);
        else
            return 0;
    }

    /**
        @brief Adds a new console command of this class.
        @param executor The executor of the command
        @param bCreateShortcut If this is true a shortcut gets created so you don't have to add the classname to access this command
        @return The executor of the command
    */
    ExecutorStatic& Identifier::addConsoleCommand(ExecutorStatic* executor, bool bCreateShortcut)
    {
        this->bHasConsoleCommands_ = true;
        this->consoleCommands_[executor->getName()] = executor;
        this->consoleCommands_LC_[getLowercase(executor->getName())] = executor;

        if (bCreateShortcut)
            CommandExecutor::addConsoleCommandShortcut(executor);

        return (*executor);
    }

    /**
        @brief Returns the executor of a console command with given name.
        @brief name The name of the requested console command
        @return The executor of the requested console command
    */
    ExecutorStatic* Identifier::getConsoleCommand(const std::string& name) const
    {
        std::map<std::string, ExecutorStatic*>::const_iterator it = this->consoleCommands_.find(name);
        if (it != this->consoleCommands_.end())
            return (*it).second;
        else
            return 0;
    }

    /**
        @brief Returns the executor of a console command with given name in lowercase.
        @brief name The name of the requested console command in lowercae
        @return The executor of the requested console command
    */
    ExecutorStatic* Identifier::getLowercaseConsoleCommand(const std::string& name) const
    {
        std::map<std::string, ExecutorStatic*>::const_iterator it = this->consoleCommands_LC_.find(name);
        if (it != this->consoleCommands_LC_.end())
            return (*it).second;
        else
            return 0;
    }

    /**
        @brief Lists the names of all Identifiers in a std::set<const Identifier*>.
        @param out The outstream
        @param list The list (or set) of Identifiers
        @return The outstream
    */
    std::ostream& operator<<(std::ostream& out, const std::set<const Identifier*>& list)
    {
        for (std::set<const Identifier*>::const_iterator it = list.begin(); it != list.end(); ++it)
            out << (*it)->getName() << " ";

        return out;
    }
}
