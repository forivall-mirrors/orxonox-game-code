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

/**
    @file
    @brief Implementation of the Identifier class.
*/

#include "Identifier.h"

#include <ostream>

#include "util/StringUtils.h"
#include "ConfigValueContainer.h"
#include "ConsoleCommand.h"
#include "ClassFactory.h"
#include "XMLPort.h"

namespace orxonox
{
    // ###############################
    // ###       Identifier        ###
    // ###############################
    int Identifier::hierarchyCreatingCounter_s = 0;
    unsigned int Identifier::classIDCounter_s = 0;

    /**
        @brief Constructor: No factory, no object created, new ObjectList and a unique networkID.
    */
    Identifier::Identifier()
        : classID_(classIDCounter_s++)
    {
        this->objects_ = new ObjectListBase(this);

        this->bCreatedOneObject_ = false;
        this->bSetName_ = false;
        this->factory_ = 0;
        this->bLoadable_ = false;

        this->bHasConfigValues_ = false;
        this->bHasConsoleCommands_ = false;

        // Default network ID is the class ID
        this->networkID_ = this->classID_;
    }

    /**
        @brief Destructor: Deletes the list containing the children.
    */
    Identifier::~Identifier()
    {
        delete this->objects_;

        if (this->factory_)
            delete this->factory_;

        for (std::map<std::string, ConsoleCommand*>::iterator it = this->consoleCommands_.begin(); it != this->consoleCommands_.end(); ++it)
            delete (it->second);
        for (std::map<std::string, ConfigValueContainer*>::iterator it = this->configValues_.begin(); it != this->configValues_.end(); ++it)
            delete (it->second);
        for (std::map<std::string, XMLPortParamContainer*>::iterator it = this->xmlportParamContainers_.begin(); it != this->xmlportParamContainers_.end(); ++it)
            delete (it->second);
        for (std::map<std::string, XMLPortObjectContainer*>::iterator it = this->xmlportObjectContainers_.begin(); it != this->xmlportObjectContainers_.end(); ++it)
            delete (it->second);
        for (std::map<std::string, XMLPortObjectContainer*>::iterator it = this->xmlportEventContainers_.begin(); it != this->xmlportEventContainers_.end(); ++it)
            delete (it->second);
    }

    /**
        @brief Returns the identifier map with the names as received by typeid(). This is only used internally.
    */
    std::map<std::string, Identifier*>& Identifier::getTypeIDIdentifierMap()
    {
        static std::map<std::string, Identifier*> identifiers;    //!< The map to store all Identifiers.
        return identifiers;
    }

    /**
        @brief Returns an identifier by name and adds it if not available
        @param name The name of the identifier as typeid().name() suggests
        @param proposal A pointer to a newly created identifier for the case of non existance in the map
        @return The identifier (unique instance)
    */
    Identifier* Identifier::getIdentifierSingleton(const std::string& name, Identifier* proposal)
    {
        std::map<std::string, Identifier*>::const_iterator it = getTypeIDIdentifierMap().find(name);

        if (it != getTypeIDIdentifierMap().end())
        {
            // There is already an entry: return it and delete the proposal
            delete proposal;
            return it->second;
        }
        else
        {
            // There is no entry: put the proposal into the map and return it
            getTypeIDIdentifierMap()[name] = proposal;
            return proposal;
        }
    }

    /**
        @brief Registers a class, which means that the name and the parents get stored.
        @param parents A list, containing the Identifiers of all parents of the class
        @param bRootClass True if the class is either an Interface or the BaseObject itself
    */
    void Identifier::initializeClassHierarchy(std::set<const Identifier*>* parents, bool bRootClass)
    {
        // Check if at least one object of the given type was created
        if (!this->bCreatedOneObject_ && Identifier::isCreatingHierarchy())
        {
            // If no: We have to store the information and initialize the Identifier
            COUT(4) << "*** ClassIdentifier: Register Class in " << this->getName() << "-Singleton -> Initialize Singleton." << std::endl;
            if (bRootClass)
                this->initialize(0); // If a class is derived from two interfaces, the second interface might think it's derived from the first because of the order of constructor-calls. Thats why we set parents to zero in that case.
            else
                this->initialize(parents);
        }
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
                (*it)->children_.insert((*it)->children_.end(), this);

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
                (*it)->directChildren_.insert((*it)->directChildren_.end(), this);

                // Create the super-function dependencies
                (*it)->createSuperFunctionCaller();
            }
        }
    }

    /**
        @brief Creates the class-hierarchy by creating and destroying one object of each type.
    */
    void Identifier::createClassHierarchy()
    {
        COUT(3) << "*** Identifier: Create class-hierarchy" << std::endl;
        Identifier::startCreatingHierarchy();
        for (std::map<std::string, Identifier*>::const_iterator it = Identifier::getStringIdentifierMap().begin(); it != Identifier::getStringIdentifierMap().end(); ++it)
        {
            // To create the new branch of the class-hierarchy, we create a new object and delete it afterwards.
            if (it->second->hasFactory())
            {
                BaseObject* temp = it->second->fabricate(0);
                temp->destroy();
            }
        }
        Identifier::stopCreatingHierarchy();
        COUT(3) << "*** Identifier: Finished class-hierarchy creation" << std::endl;
    }

    /**
        @brief Destroys all Identifiers. Called when exiting the program.
    */
    void Identifier::destroyAllIdentifiers()
    {
        for (std::map<std::string, Identifier*>::iterator it = Identifier::getTypeIDIdentifierMap().begin(); it != Identifier::getTypeIDIdentifierMap().end(); ++it)
            delete (it->second);
    }

    /**
        @brief Sets the name of the class.
        @param name The name
    */
    void Identifier::setName(const std::string& name)
    {
        if (!this->bSetName_)
        {
            this->name_ = name;
            this->bSetName_ = true;
            Identifier::getStringIdentifierMapIntern()[name] = this;
            Identifier::getLowercaseStringIdentifierMapIntern()[getLowercase(name)] = this;
            Identifier::getIDIdentifierMapIntern()[this->networkID_] = this;
        }
    }

    /**
        @brief Creates an object of the type the Identifier belongs to.
        @return The new object
    */
    BaseObject* Identifier::fabricate(BaseObject* creator)
    {
        if (this->factory_)
        {
            return this->factory_->fabricate(creator); // We have to return a BaseObject, because we don't know the exact type.
        }
        else
        {
            COUT(1) << "An error occurred in Identifier.cc:" << std::endl;
            COUT(1) << "Error: Cannot fabricate an object of type '" << this->name_ << "'. Class has no factory." << std::endl;
            COUT(1) << "Aborting..." << std::endl;
            abort();
            return 0;
        }
    }

    /**
        @brief Sets the network ID to a new value and changes the entry in the ID-Identifier-map.
        @param id The new network ID
    */
    void Identifier::setNetworkID(uint32_t id)
    {
//        Identifier::getIDIdentifierMapIntern().erase(this->networkID_);
        Identifier::getIDIdentifierMapIntern()[id] = this;
        this->networkID_ = id;
    }

    /**
        @brief Returns true, if the Identifier is at least of the given type.
        @param identifier The identifier to compare with
    */
    bool Identifier::isA(const Identifier* identifier) const
    {
        return (identifier == this || (this->parents_.find(identifier) != this->parents_.end()));
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
        return (this->parents_.find(identifier) != this->parents_.end());
    }

    /**
        @brief Returns true, if the assigned identifier is a direct child of the given identifier.
        @param identifier The identifier to compare with
    */
    bool Identifier::isDirectChildOf(const Identifier* identifier) const
    {
        return (this->directParents_.find(identifier) != this->directParents_.end());
    }

    /**
        @brief Returns true, if the assigned identifier is a parent of the given identifier.
        @param identifier The identifier to compare with
    */
    bool Identifier::isParentOf(const Identifier* identifier) const
    {
        return (this->children_.find(identifier) != this->children_.end());
    }

    /**
        @brief Returns true, if the assigned identifier is a direct parent of the given identifier.
        @param identifier The identifier to compare with
    */
    bool Identifier::isDirectParentOf(const Identifier* identifier) const
    {
        return (this->directChildren_.find(identifier) != this->directChildren_.end());
    }

    /**
        @brief Returns the map that stores all Identifiers with their names.
        @return The map
    */
    std::map<std::string, Identifier*>& Identifier::getStringIdentifierMapIntern()
    {
        static std::map<std::string, Identifier*> identifierMap;
        return identifierMap;
    }

    /**
        @brief Returns the map that stores all Identifiers with their names in lowercase.
        @return The map
    */
    std::map<std::string, Identifier*>& Identifier::getLowercaseStringIdentifierMapIntern()
    {
        static std::map<std::string, Identifier*> lowercaseIdentifierMap;
        return lowercaseIdentifierMap;
    }

    /**
        @brief Returns the map that stores all Identifiers with their network IDs.
        @return The map
    */
    std::map<uint32_t, Identifier*>& Identifier::getIDIdentifierMapIntern()
    {
        static std::map<uint32_t, Identifier*> identifierMap;
        return identifierMap;
    }

    /**
        @brief Returns the Identifier with a given name.
        @param name The name of the wanted Identifier
        @return The Identifier
    */
    Identifier* Identifier::getIdentifierByString(const std::string& name)
    {
        std::map<std::string, Identifier*>::const_iterator it = Identifier::getStringIdentifierMapIntern().find(name);
        if (it != Identifier::getStringIdentifierMapIntern().end())
            return it->second;
        else
            return 0;
    }

    /**
        @brief Returns the Identifier with a given name in lowercase.
        @param name The name of the wanted Identifier
        @return The Identifier
    */
    Identifier* Identifier::getIdentifierByLowercaseString(const std::string& name)
    {
        std::map<std::string, Identifier*>::const_iterator it = Identifier::getLowercaseStringIdentifierMapIntern().find(name);
        if (it != Identifier::getLowercaseStringIdentifierMapIntern().end())
            return it->second;
        else
            return 0;
    }

    /**
        @brief Returns the Identifier with a given network ID.
        @param id The network ID of the wanted Identifier
        @return The Identifier
    */
    Identifier* Identifier::getIdentifierByID(const uint32_t id)
    {
        std::map<uint32_t, Identifier*>::const_iterator it = Identifier::getIDIdentifierMapIntern().find(id);
        if (it != Identifier::getIDIdentifierMapIntern().end())
            return it->second;
        else
            return 0;
    }

    /**
        @brief Cleans the NetworkID map (needed on clients for correct initialization)
    */
    void Identifier::clearNetworkIDs()
    {
        Identifier::getIDIdentifierMapIntern().clear();
    }

    /**
        @brief Adds the ConfigValueContainer of a variable, given by the string of its name.
        @param varname The name of the variablee
        @param container The container
    */
    void Identifier::addConfigValueContainer(const std::string& varname, ConfigValueContainer* container)
    {
        std::map<std::string, ConfigValueContainer*>::const_iterator it = this->configValues_.find(varname);
        if (it != this->configValues_.end())
        {
            COUT(2) << "Warning: Overwriting config-value with name " << varname << " in class " << this->getName() << "." << std::endl;
            delete (it->second);
        }

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
            return it->second;
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
            return it->second;
        else
            return 0;
    }

    /**
        @brief Adds a new console command of this class.
        @param executor The executor of the command
        @param bCreateShortcut If this is true a shortcut gets created so you don't have to add the classname to access this command
        @return The executor of the command
    */
    ConsoleCommand& Identifier::addConsoleCommand(ConsoleCommand* command, bool bCreateShortcut)
    {
        std::map<std::string, ConsoleCommand*>::const_iterator it = this->consoleCommands_.find(command->getName());
        if (it != this->consoleCommands_.end())
        {
            COUT(2) << "Warning: Overwriting console-command with name " << command->getName() << " in class " << this->getName() << "." << std::endl;
            delete (it->second);
        }

        this->bHasConsoleCommands_ = true;
        this->consoleCommands_[command->getName()] = command;
        this->consoleCommands_LC_[getLowercase(command->getName())] = command;

        if (bCreateShortcut)
            CommandExecutor::addConsoleCommandShortcut(command);

        return (*command);
    }

    /**
        @brief Returns the executor of a console command with given name.
        @brief name The name of the requested console command
        @return The executor of the requested console command
    */
    ConsoleCommand* Identifier::getConsoleCommand(const std::string& name) const
    {
        std::map<std::string, ConsoleCommand*>::const_iterator it = this->consoleCommands_.find(name);
        if (it != this->consoleCommands_.end())
            return it->second;
        else
            return 0;
    }

    /**
        @brief Returns the executor of a console command with given name in lowercase.
        @brief name The name of the requested console command in lowercae
        @return The executor of the requested console command
    */
    ConsoleCommand* Identifier::getLowercaseConsoleCommand(const std::string& name) const
    {
        std::map<std::string, ConsoleCommand*>::const_iterator it = this->consoleCommands_LC_.find(name);
        if (it != this->consoleCommands_LC_.end())
            return it->second;
        else
            return 0;
    }

    /**
        @brief Returns a XMLPortParamContainer that loads a parameter of this class.
        @param paramname The name of the parameter
        @return The container
    */
    XMLPortParamContainer* Identifier::getXMLPortParamContainer(const std::string& paramname)
    {
        std::map<std::string, XMLPortParamContainer*>::const_iterator it = this->xmlportParamContainers_.find(paramname);
        if (it != this->xmlportParamContainers_.end())
            return it->second;
        else
            return 0;
    }

    /**
        @brief Adds a new XMLPortParamContainer that loads a parameter of this class.
        @param paramname The name of the parameter
        @param container The container
    */
    void Identifier::addXMLPortParamContainer(const std::string& paramname, XMLPortParamContainer* container)
    {
        std::map<std::string, XMLPortParamContainer*>::const_iterator it = this->xmlportParamContainers_.find(paramname);
        if (it != this->xmlportParamContainers_.end())
        {
            COUT(2) << "Warning: Overwriting XMLPortParamContainer in class " << this->getName() << "." << std::endl;
            delete (it->second);
        }

        this->xmlportParamContainers_[paramname] = container;
    }

    /**
        @brief Returns a XMLPortObjectContainer that attaches an object to this class.
        @param sectionname The name of the section that contains the attachable objects
        @return The container
    */
    XMLPortObjectContainer* Identifier::getXMLPortObjectContainer(const std::string& sectionname)
    {
        std::map<std::string, XMLPortObjectContainer*>::const_iterator it = this->xmlportObjectContainers_.find(sectionname);
        if (it != this->xmlportObjectContainers_.end())
            return it->second;
        else
            return 0;
    }

    /**
        @brief Adds a new XMLPortObjectContainer that attaches an object to this class.
        @param sectionname The name of the section that contains the attachable objects
        @param container The container
    */
    void Identifier::addXMLPortObjectContainer(const std::string& sectionname, XMLPortObjectContainer* container)
    {
        std::map<std::string, XMLPortObjectContainer*>::const_iterator it = this->xmlportObjectContainers_.find(sectionname);
        if (it != this->xmlportObjectContainers_.end())
        {
            COUT(2) << "Warning: Overwriting XMLPortObjectContainer in class " << this->getName() << "." << std::endl;
            delete (it->second);
        }

        this->xmlportObjectContainers_[sectionname] = container;
    }

    /**
        @brief Returns a XMLPortEventContainer that attaches an event to this class.
        @param sectionname The name of the section that contains the event
        @return The container
    */
    XMLPortObjectContainer* Identifier::getXMLPortEventContainer(const std::string& eventname)
    {
        std::map<std::string, XMLPortObjectContainer*>::const_iterator it = this->xmlportEventContainers_.find(eventname);
        if (it != this->xmlportEventContainers_.end())
            return it->second;
        else
            return 0;
    }

    /**
        @brief Adds a new XMLPortEventContainer that attaches an event to this class.
        @param sectionname The name of the section that contains the event
        @param container The container
    */
    void Identifier::addXMLPortEventContainer(const std::string& eventname, XMLPortObjectContainer* container)
    {
        std::map<std::string, XMLPortObjectContainer*>::const_iterator it = this->xmlportEventContainers_.find(eventname);
        if (it != this->xmlportEventContainers_.end())
        {
            COUT(2) << "Warning: Overwriting XMLPortEventContainer in class " << this->getName() << "." << std::endl;
            delete (it->second);
        }

        this->xmlportEventContainers_[eventname] = container;
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
        {
            if (it != list.begin())
                out << " ";
            out << (*it)->getName();
        }

        return out;
    }
}
