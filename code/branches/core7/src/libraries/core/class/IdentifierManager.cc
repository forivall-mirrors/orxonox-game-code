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

#include "IdentifierManager.h"

#include <ostream>

#include "util/StringUtils.h"
#include "core/Core.h"
#include "core/config/ConfigValueContainer.h"
#include "core/XMLPort.h"
#include "core/object/ClassFactory.h"

namespace orxonox
{
    /* static */ IdentifierManager& IdentifierManager::getInstance()
    {
        static IdentifierManager instance;
        return instance;
    }

    IdentifierManager::IdentifierManager()
    {
        this->hierarchyCreatingCounter_s = 0;
        this->classIDCounter_s = 0;
    }

    /**
        @brief Returns an identifier by name and adds it if not available
        @param proposal A pointer to a newly created identifier for the case of non existence in the map
        @return The identifier (unique instance)
    */
    Identifier* IdentifierManager::getGloballyUniqueIdentifier(Identifier* proposal)
    {
        const std::string& typeidName = proposal->getTypeidName();
        std::map<std::string, Identifier*>::const_iterator it = this->identifierByTypeidName_.find(typeidName);

        if (it != this->identifierByTypeidName_.end())
        {
            // There is already an entry: return it
            return it->second;
        }
        else
        {
            // There is no entry: put the proposal into the map and return it
            this->identifierByTypeidName_[typeidName] = proposal;
            return proposal;
        }
    }

    /**
     * Registers the identifier in all maps of the IdentifierManager.
     */
    void IdentifierManager::addIdentifierToLookupMaps(Identifier* identifier)
    {
        const std::string& typeidName = identifier->getTypeidName();
        if (this->identifierByTypeidName_.find(typeidName) != this->identifierByTypeidName_.end())
        {
            this->identifierByString_[identifier->getName()] = identifier;
            this->identifierByLowercaseString_[getLowercase(identifier->getName())] = identifier;
            this->identifierByNetworkId_[identifier->getNetworkID()] = identifier;
        }
        else
            orxout(internal_warning) << "Trying to add an identifier to lookup maps which is not known to IdentifierManager" << endl;
    }

    /**
        @brief Creates the class-hierarchy by creating and destroying one object of each type.
    */
    void IdentifierManager::createClassHierarchy()
    {
        orxout(internal_status) << "Create class-hierarchy" << endl;
        this->startCreatingHierarchy();

        std::set<Identifier*> initializedIdentifiers;

        // ensure root context exists before starting to create objects. if the root context is dynamically created while creating the class hierarchy, we
        // would mistakenly assume the class of the currently created object inherits from Context
        Context::getRootContext();

        // iterate over all identifiers, create one instance of each class and initialize the identifiers
        {
            Context temporaryContext(NULL);
            for (std::map<std::string, Identifier*>::const_iterator it = this->identifierByTypeidName_.begin(); it != this->identifierByTypeidName_.end(); ++it)
            {
                orxout(verbose, context::identifier) << "Initialize ClassIdentifier<" << it->second->getName() << ">-Singleton." << endl;
                // To initialize the identifier, we create a new object and delete it afterwards.
                if (it->second->hasFactory())
                {
                    this->identifiersOfNewObject_.clear();
                    Identifiable* temp = it->second->fabricate(&temporaryContext);
                    if (temp->getIdentifier() != it->second)
                        orxout(internal_error) << "Newly created object of type " << it->second->getName() << " has unexpected identifier. Did you forget to use RegisterObject(classname)?" << endl;

                    it->second->initializeParents(temp, this->identifiersOfNewObject_);

                    delete temp;
                }
                else
                    it->second->initializeDirectParentsOfAbstractClass();

                initializedIdentifiers.insert(it->second);
            }

            size_t numberOfObjects = temporaryContext.getObjectList<Listable>()->size();
            if (numberOfObjects > 0)
                orxout(internal_warning) << "There are still " << numberOfObjects << " listables left after creating the class hierarchy" << endl;
        }

        // finish the initialization of all identifiers
        for (std::map<std::string, Identifier*>::const_iterator it = this->identifierByTypeidName_.begin(); it != this->identifierByTypeidName_.end(); ++it)
        {
            if (initializedIdentifiers.find(it->second) != initializedIdentifiers.end())
                it->second->finishInitialization();
            else
                orxout(internal_error) << "Identifier was registered late and is not initialized: " << it->second->getName() << " / " << it->second->getTypeidName() << endl;
        }

        // only check class hierarchy in dev mode because it's an expensive operation and it requires a developer to fix detected problems anyway.
        if (!Core::exists() || Core::getInstance().inDevMode())
            this->verifyClassHierarchy();

        this->stopCreatingHierarchy();
        orxout(internal_status) << "Finished class-hierarchy creation" << endl;
    }

    /**
     * Verifies if the class hierarchy is consistent with the RTTI.
     */
    void IdentifierManager::verifyClassHierarchy()
    {
        Context temporaryContext(NULL);
        for (std::map<std::string, Identifier*>::const_iterator it1 = this->identifierByTypeidName_.begin(); it1 != this->identifierByTypeidName_.end(); ++it1)
        {
            if (!it1->second->hasFactory())
                continue;

            Identifiable* temp = it1->second->fabricate(&temporaryContext);

            for (std::map<std::string, Identifier*>::const_iterator it2 = this->identifierByTypeidName_.begin(); it2 != this->identifierByTypeidName_.end(); ++it2)
            {
                bool isA_AccordingToRtti = it2->second->canDynamicCastObjectToIdentifierClass(temp);
                bool isA_AccordingToClassHierarchy = temp->isA(it2->second);

                if (isA_AccordingToRtti != isA_AccordingToClassHierarchy)
                {
                    orxout(internal_error) << "Class hierarchy does not match RTTI: Class hierarchy claims that " << it1->second->getName() <<
                        (isA_AccordingToClassHierarchy ? " is a " : " is not a ") << it2->second->getName() << " but RTTI says the opposite." << endl;
                }
            }

            delete temp;
        }
        orxout(internal_info) << "Class hierarchy matches RTTI" << endl;

        size_t numberOfObjects = temporaryContext.getObjectList<Listable>()->size();
        if (numberOfObjects > 0)
            orxout(internal_warning) << "There are still " << numberOfObjects << " listables left after creating the class hierarchy" << endl;
    }

    /**
        @brief Destroys all Identifiers. Called when exiting the program.
    */
    void IdentifierManager::destroyAllIdentifiers()
    {
        for (std::map<std::string, Identifier*>::iterator it = this->identifierByTypeidName_.begin(); it != this->identifierByTypeidName_.end(); ++it)
            delete (it->second);

        this->identifierByTypeidName_.clear();
        this->identifierByString_.clear();
        this->identifierByLowercaseString_.clear();
        this->identifierByNetworkId_.clear();
    }

    /**
     * @brief Notifies the IdentifierManager about a newly created object while creating the class hierarchy.
     */
    void IdentifierManager::createdObject(Identifiable* identifiable)
    {
        if (this->isCreatingHierarchy())
            this->identifiersOfNewObject_.insert(identifiable->getIdentifier());
        else
            orxout(internal_warning) << "createdObject() called outside of class hierarchy creation" << endl;
    }

    /**
        @brief Returns the Identifier with a given name.
        @param name The name of the wanted Identifier
        @return The Identifier
    */
    Identifier* IdentifierManager::getIdentifierByString(const std::string& name)
    {
        std::map<std::string, Identifier*>::const_iterator it = this->identifierByString_.find(name);
        if (it != this->identifierByString_.end())
            return it->second;
        else
            return 0;
    }

    /**
        @brief Returns the Identifier with a given name in lowercase.
        @param name The name of the wanted Identifier
        @return The Identifier
    */
    Identifier* IdentifierManager::getIdentifierByLowercaseString(const std::string& name)
    {
        std::map<std::string, Identifier*>::const_iterator it = this->identifierByLowercaseString_.find(name);
        if (it != this->identifierByLowercaseString_.end())
            return it->second;
        else
            return 0;
    }

    /**
        @brief Returns the Identifier with a given network ID.
        @param id The network ID of the wanted Identifier
        @return The Identifier
    */
    Identifier* IdentifierManager::getIdentifierByID(const uint32_t id)
    {
        std::map<uint32_t, Identifier*>::const_iterator it = this->identifierByNetworkId_.find(id);
        if (it != this->identifierByNetworkId_.end())
            return it->second;
        else
            return 0;
    }

    /**
        @brief Cleans the NetworkID map (needed on clients for correct initialization)
    */
    void IdentifierManager::clearNetworkIDs()
    {
        this->identifierByNetworkId_.clear();
    }
}
