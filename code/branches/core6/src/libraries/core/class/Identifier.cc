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
#include "core/config/ConfigValueContainer.h"
#include "core/XMLPort.h"
#include "core/object/ClassFactory.h"

namespace orxonox
{
    // ###############################
    // ###       Identifier        ###
    // ###############################
    /**
        @brief Constructor: No factory, no object created, new ObjectList and a unique networkID.
    */
    Identifier::Identifier()
        : classID_(IdentifierManager::classIDCounter_s++)
    {
        this->objects_ = new ObjectListBase();

        this->bCreatedOneObject_ = false;
        this->bSetName_ = false;
        this->factory_ = 0;
        this->bLoadable_ = false;

        this->bHasConfigValues_ = false;

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

        for (std::map<std::string, ConfigValueContainer*>::iterator it = this->configValues_.begin(); it != this->configValues_.end(); ++it)
            delete (it->second);
        for (std::map<std::string, XMLPortParamContainer*>::iterator it = this->xmlportParamContainers_.begin(); it != this->xmlportParamContainers_.end(); ++it)
            delete (it->second);
        for (std::map<std::string, XMLPortObjectContainer*>::iterator it = this->xmlportObjectContainers_.begin(); it != this->xmlportObjectContainers_.end(); ++it)
            delete (it->second);
    }

    /**
        @brief Registers a class, which means that the name and the parents get stored.
        @param parents A list, containing the Identifiers of all parents of the class
        @param bRootClass True if the class is either an Interface or the BaseObject itself
    */
    void Identifier::initializeClassHierarchy(std::set<const Identifier*>* parents, bool bRootClass)
    {
        // Check if at least one object of the given type was created
        if (!this->bCreatedOneObject_ && IdentifierManager::isCreatingHierarchy())
        {
            // If no: We have to store the information and initialize the Identifier
            orxout(verbose, context::identifier) << "Register Class in ClassIdentifier<" << this->getName() << ">-Singleton -> Initialize Singleton." << endl;
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
        orxout(verbose, context::identifier) << "Initialize ClassIdentifier<" << this->name_ << ">-Singleton." << endl;
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
        @brief Sets the name of the class.
    */
    void Identifier::setName(const std::string& name)
    {
        if (!this->bSetName_)
        {
            this->name_ = name;
            this->bSetName_ = true;
            IdentifierManager::getStringIdentifierMapIntern()[name] = this;
            IdentifierManager::getLowercaseStringIdentifierMapIntern()[getLowercase(name)] = this;
            IdentifierManager::getIDIdentifierMapIntern()[this->networkID_] = this;
        }
    }

    void Identifier::setFactory(Factory* factory)
    {
        if (this->factory_)
            delete this->factory_;

        this->factory_ = factory;
    }


    /**
        @brief Creates an object of the type the Identifier belongs to.
        @return The new object
    */
    OrxonoxClass* Identifier::fabricate(BaseObject* creator)
    {
        if (this->factory_)
        {
            return this->factory_->fabricate(creator);
        }
        else
        {
            orxout(user_error) << "An error occurred in Identifier.cc:" << endl;
            orxout(user_error) << "Cannot fabricate an object of type '" << this->name_ << "'. Class has no factory." << endl;
            orxout(user_error) << "Aborting..." << endl;
            abort();
            return 0;
        }
    }

    /**
        @brief Sets the network ID to a new value and changes the entry in the ID-Identifier-map.
    */
    void Identifier::setNetworkID(uint32_t id)
    {
//        Identifier::getIDIdentifierMapIntern().erase(this->networkID_);
        IdentifierManager::getIDIdentifierMapIntern()[id] = this;
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
        @brief Adds the ConfigValueContainer of a variable, given by the string of its name.
        @param varname The name of the variablee
        @param container The container
    */
    void Identifier::addConfigValueContainer(const std::string& varname, ConfigValueContainer* container)
    {
        std::map<std::string, ConfigValueContainer*>::const_iterator it = this->configValues_.find(varname);
        if (it != this->configValues_.end())
        {
            orxout(internal_warning) << "Overwriting config-value with name " << varname << " in class " << this->getName() << '.' << endl;
            delete (it->second);
        }

        this->bHasConfigValues_ = true;
        this->configValues_[varname] = container;
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
            orxout(internal_warning) << "Overwriting XMLPortParamContainer in class " << this->getName() << '.' << endl;
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
            orxout(internal_warning) << "Overwriting XMLPortObjectContainer in class " << this->getName() << '.' << endl;
            delete (it->second);
        }

        this->xmlportObjectContainers_[sectionname] = container;
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
                out << ' ';
            out << (*it)->getName();
        }

        return out;
    }
}
