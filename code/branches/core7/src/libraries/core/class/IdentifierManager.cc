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
        this->recordTraceForIdentifier_ = NULL;
    }

    /**
     * Registers the identifier in all maps of the IdentifierManager.
     */
    void IdentifierManager::addIdentifier(Identifier* identifier)
    {
        orxout(verbose, context::identifier) << "Adding identifier for " << identifier->getName() << " / " << identifier->getTypeInfo().name() << endl;

        this->identifiers_.insert(identifier);
        this->identifierByString_[identifier->getName()] = identifier;
        this->identifierByLowercaseString_[getLowercase(identifier->getName())] = identifier;
        this->identifierByNetworkId_[identifier->getNetworkID()] = identifier;
    }

    /**
     * Unregisters the identifier from all maps of the IdentifierManager.
     */
    void IdentifierManager::removeIdentifier(Identifier* identifier)
    {
        this->identifiers_.erase(identifier);
        this->identifierByString_.erase(identifier->getName());
        this->identifierByLowercaseString_.erase(getLowercase(identifier->getName()));
        this->identifierByNetworkId_.erase(identifier->getNetworkID());
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
            for (std::set<Identifier*>::const_iterator it = this->identifiers_.begin(); it != this->identifiers_.end(); ++it)
            {
                Identifier* identifier = (*it);

                orxout(verbose, context::identifier) << "Initialize ClassIdentifier<" << identifier->getName() << ">-Singleton." << endl;
                // To initialize the identifier, we create a new object and delete it afterwards.
                if (identifier->hasFactory())
                {
                    this->identifierTraceOfNewObject_.clear();
                    this->recordTraceForIdentifier_ = identifier;

                    Identifiable* temp = identifier->fabricate(&temporaryContext);

                    this->recordTraceForIdentifier_ = NULL;

                    if (temp->getIdentifier() != identifier)
                        orxout(internal_error) << "Newly created object of type " << identifier->getName() << " has unexpected identifier. Did you forget to use RegisterObject(classname)?" << endl;

                    identifier->initializeParents(this->identifierTraceOfNewObject_[temp]);

                    delete temp;
                }

                initializedIdentifiers.insert(identifier);
            }

            size_t numberOfObjects = temporaryContext.getObjectList<Listable>()->size();
            if (numberOfObjects > 0)
                orxout(internal_warning) << "There are still " << numberOfObjects << " listables left after creating the class hierarchy" << endl;
        }

        // finish the initialization of all identifiers
        for (std::set<Identifier*>::const_iterator it = this->identifiers_.begin(); it != this->identifiers_.end(); ++it)
        {
            Identifier* identifier = (*it);

            if (initializedIdentifiers.find(identifier) != initializedIdentifiers.end())
                identifier->finishInitialization();
            else
                orxout(internal_error) << "Identifier was registered late and is not initialized: " << identifier->getName() << " / " << identifier->getTypeInfo().name() << endl;
        }

        // only check class hierarchy in dev mode because it's an expensive operation and it requires a developer to fix detected problems anyway.
        if (!Core::exists() || Core::getInstance().getConfig()->inDevMode())
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
        for (std::set<Identifier*>::const_iterator it1 = this->identifiers_.begin(); it1 != this->identifiers_.end(); ++it1)
        {
            if (!(*it1)->hasFactory())
                continue;

            Identifiable* temp = (*it1)->fabricate(&temporaryContext);

            for (std::set<Identifier*>::const_iterator it2 = this->identifiers_.begin(); it2 != this->identifiers_.end(); ++it2)
            {
                bool isA_AccordingToRtti = (*it2)->canDynamicCastObjectToIdentifierClass(temp);
                bool isA_AccordingToClassHierarchy = temp->isA((*it2));

                if (isA_AccordingToRtti != isA_AccordingToClassHierarchy)
                {
                    orxout(internal_error) << "Class hierarchy does not match RTTI: Class hierarchy claims that " << (*it1)->getName() <<
                        (isA_AccordingToClassHierarchy ? " is a " : " is not a ") << (*it2)->getName() << " but RTTI says the opposite." << endl;
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
     * @brief Resets all Identifiers.
     */
    void IdentifierManager::destroyClassHierarchy()
    {
        orxout(internal_status) << "Destroy class-hierarchy" << endl;
        for (std::set<Identifier*>::const_iterator it = this->identifiers_.begin(); it != this->identifiers_.end(); ++it)
            (*it)->reset();
    }

    /**
     * @brief Notifies the IdentifierManager about a newly created object while creating the class hierarchy.
     */
    void IdentifierManager::createdObject(Identifiable* identifiable)
    {
        if (this->isCreatingHierarchy())
        {
            if (this->recordTraceForIdentifier_)
            {
                std::list<const Identifier*>& traceForObject = this->identifierTraceOfNewObject_[identifiable];
                if (std::find(traceForObject.begin(), traceForObject.end(), identifiable->getIdentifier()) != traceForObject.end())
                {
                    orxout(internal_warning) << this->recordTraceForIdentifier_->getName() << " inherits two times from " <<
                        identifiable->getIdentifier()->getName() << ". Did you forget to use virtual inheritance?" << endl;
                }
                traceForObject.push_back(identifiable->getIdentifier());
            }
        }
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
        @brief Returns the Identifier with a given typeid-name.
        @param name The typeid-name of the wanted Identifier
        @return The Identifier
    */
    Identifier* IdentifierManager::getIdentifierByTypeInfo(const std::type_info& typeInfo)
    {
        // TODO: use std::type_index and a map to find identifiers by type_info (only with c++11)
        for (std::set<Identifier*>::iterator it = this->identifiers_.begin(); it != this->identifiers_.end(); ++it)
            if ((*it)->getTypeInfo() == typeInfo)
                return (*it);
        return 0;
    }

    /**
        @brief Cleans the NetworkID map (needed on clients for correct initialization)
    */
    void IdentifierManager::clearNetworkIDs()
    {
        this->identifierByNetworkId_.clear();
    }

    /**
        @brief Destroys all Identifiers. Called when exiting the program.
    */
    void IdentifierManager::destroyAllIdentifiers()
    {
        for (std::set<Identifier*>::iterator it = this->identifiers_.begin(); it != this->identifiers_.end(); ++it)
            delete (*it);

        this->identifiers_.clear();
        this->identifierByString_.clear();
        this->identifierByLowercaseString_.clear();
        this->identifierByNetworkId_.clear();
    }
}
