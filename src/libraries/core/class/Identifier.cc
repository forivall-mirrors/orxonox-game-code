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
#include "core/CoreIncludes.h"
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
        : classID_(IdentifierManager::getInstance().getUniqueClassId())
    {
        this->factory_ = 0;
        this->bInitialized_ = false;
        this->bLoadable_ = false;
        this->bIsVirtualBase_ = false;

        this->bHasConfigValues_ = false;

        // Default network ID is the class ID
        this->networkID_ = this->classID_;
    }

    /**
        @brief Destructor: Deletes the list containing the children.
    */
    Identifier::~Identifier()
    {
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
        @brief Sets the name of the class.
    */
    void Identifier::setName(const std::string& name)
    {
        if (name != this->name_)
        {
            this->name_ = name;
            IdentifierManager::getInstance().addIdentifierToLookupMaps(this);
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
    Identifiable* Identifier::fabricate(Context* context)
    {
        if (this->factory_)
        {
            return this->factory_->fabricate(context);
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
        this->networkID_ = id;
        IdentifierManager::getInstance().addIdentifierToLookupMaps(this);
    }

    /**
     * @brief Used to define the direct parents of an Identifier of an abstract class.
     */
    Identifier& Identifier::inheritsFrom(Identifier* directParent)
    {
        if (this->parents_.empty())
            this->directParents_.push_back(directParent);
        else
            orxout(internal_error) << "Trying to add " << directParent->getName() << " as a direct parent of " << this->getName() << " after the latter was already initialized" << endl;

        return *this;
    }

    /**
     * @brief Initializes the parents of this Identifier while creating the class hierarchy.
     * @param initializationTrace All identifiers that were recorded while creating an instance of this class (including nested classes and this identifier itself)
     */
    void Identifier::initializeParents(const std::list<const Identifier*>& initializationTrace)
    {
        if (!IdentifierManager::getInstance().isCreatingHierarchy())
        {
            orxout(internal_warning) << "Identifier::initializeParents() created outside of class hierarchy creation" << endl;
            return;
        }

        if (this->directParents_.empty())
        {
            for (std::list<const Identifier*>::const_iterator it = initializationTrace.begin(); it != initializationTrace.end(); ++it)
                if (*it != this)
                    this->parents_.push_back(*it);
        }
        else
            orxout(internal_error) << "Trying to add parents to " << this->getName() << " after it was already initialized with manual calls to inheritsFrom<Class>()." << endl;
    }

    /**
     * @brief Finishes the initialization of this Identifier after creating the class hierarchy by wiring the (direct) parent/child references correctly.
     */
    void Identifier::finishInitialization()
    {
        if (!IdentifierManager::getInstance().isCreatingHierarchy())
        {
            orxout(internal_warning) << "Identifier::finishInitialization() created outside of class hierarchy creation" << endl;
            return;
        }

        if (this->isInitialized())
            return;

        if (!this->parents_.empty())
        {
            // parents defined -> this class was initialized by creating a sample instance and recording the trace of identifiers

            // initialize all parents
            for (std::list<const Identifier*>::const_iterator it = this->parents_.begin(); it != this->parents_.end(); ++it)
                const_cast<Identifier*>(*it)->finishInitialization(); // initialize parent

            // parents of parents are no direct parents of this identifier
            this->directParents_ = this->parents_;
            for (std::list<const Identifier*>::const_iterator it_parent = this->parents_.begin(); it_parent != this->parents_.end(); ++it_parent)
                for (std::list<const Identifier*>::const_iterator it_parent_parent = const_cast<Identifier*>(*it_parent)->parents_.begin(); it_parent_parent != const_cast<Identifier*>(*it_parent)->parents_.end(); ++it_parent_parent)
                    this->directParents_.remove(*it_parent_parent);
        }
        else if (!this->directParents_.empty())
        {
            // no parents defined -> this class was manually initialized by calling inheritsFrom<Class>()

            // initialize all direct parents
            for (std::list<const Identifier*>::const_iterator it = this->directParents_.begin(); it != this->directParents_.end(); ++it)
                const_cast<Identifier*>(*it)->finishInitialization(); // initialize parent

            // direct parents and their parents are also parents of this identifier (but only add them once)
            for (std::list<const Identifier*>::const_iterator it_parent = this->directParents_.begin(); it_parent != this->directParents_.end(); ++it_parent)
            {
                for (std::list<const Identifier*>::const_iterator it_parent_parent = const_cast<Identifier*>(*it_parent)->parents_.begin(); it_parent_parent != const_cast<Identifier*>(*it_parent)->parents_.end(); ++it_parent_parent)
                    if (std::find(this->parents_.begin(), this->parents_.end(), *it_parent_parent) == this->parents_.end())
                        this->parents_.push_back(*it_parent_parent);
                if (std::find(this->parents_.begin(), this->parents_.end(), *it_parent) == this->parents_.end())
                    this->parents_.push_back(*it_parent);
            }
        }
        else if (!this->isExactlyA(Class(Identifiable)))
        {
            // only Identifiable is allowed to have no parents (even tough it's currently not abstract)
            orxout(internal_error) << "Identifier " << this->getName() << " / " << this->getTypeidName() << " is marked as abstract but has no direct parents defined" << endl;
            orxout(internal_error) << "  If this class is not abstract, use RegisterClass(ThisClass);" << endl;
            orxout(internal_error) << "  If this class is abstract, use RegisterAbstractClass(ThisClass).inheritsFrom(Class(BaseClass));" << endl;
        }

        // tell all parents that this identifier is a child
        for (std::list<const Identifier*>::const_iterator it = this->parents_.begin(); it != this->parents_.end(); ++it)
            const_cast<Identifier*>(*it)->children_.insert(this);

        // tell all direct parents that this identifier is a direct child
        for (std::list<const Identifier*>::const_iterator it = this->directParents_.begin(); it != this->directParents_.end(); ++it)
        {
            const_cast<Identifier*>(*it)->directChildren_.insert(this);

            // Create the super-function dependencies
            (*it)->createSuperFunctionCaller();
        }

        this->bInitialized_ = true;
    }

    /**
        @brief Returns true, if the Identifier is at least of the given type.
        @param identifier The identifier to compare with
    */
    bool Identifier::isA(const Identifier* identifier) const
    {
        return (identifier == this || (this->isChildOf(identifier)));
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
        return (std::find(this->parents_.begin(), this->parents_.end(),  identifier) != this->parents_.end());
    }

    /**
        @brief Returns true, if the assigned identifier is a direct child of the given identifier.
        @param identifier The identifier to compare with
    */
    bool Identifier::isDirectChildOf(const Identifier* identifier) const
    {
        return (std::find(this->directParents_.begin(), this->directParents_.end(), identifier) != this->directParents_.end());
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
