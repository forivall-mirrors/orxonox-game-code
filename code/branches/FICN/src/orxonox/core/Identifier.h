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

/*!
    @file Identifier.h
    @brief Definition of the Identifier, ClassIdentifier and SubclassIdentifier classes, implementation of the ClassIdentifier and SubclassIdentifier classes.

    The Identifier contains all needed informations about the class it belongs to:
     - the name
     - a list with all objects
     - parents and childs
     - the factory (if available)
     - the networkID that can be synchronised with the server
     - all configurable variables (if available)

    Every object has a pointer to the Identifier of its class. This allows the use isA(...),
    isDirectlyA(...), isChildOf(...) and isParentOf(...).

    To create the class-hierarchy, the Identifier has some intern functions and variables.

    Every Identifier is in fact a ClassIdentifier, but they are derived from Identifier.

    SubclassIdentifier is a separated class, acting like an Identifier, but has a given class.
    You can only assign Identifiers of exactly the given class or of a derivative to a SubclassIdentifier.
*/

#ifndef _Identifier_H__
#define _Identifier_H__

#include <map>
#include <string>
#include <utility>

#include "CorePrereqs.h"

#include "ObjectList.h"
#include "IdentifierList.h"
#include "Factory.h"
#include "Debug.h"
// These two files would actually be need, but they would produce
// circular dependencies. Anyway, it does compile without them
// #include "OrxonoxClass.h"
// #include "MetaObjectList.h"

namespace orxonox
{
    class BaseObject; // Forward declaration

    // ###############################
    // ###       Identifier        ###
    // ###############################
    //! The Identifier is used to identify the class of an object and to store informations about the class.
    /**
        The Identifier contains all needed informations about the class it belongs to:
         - the name
         - a list with all objects
         - parents and childs
         - the factory (if available)
         - the networkID that can be synchronised with the server
         - all configurable variables (if available)

        Every object has a pointer to the Identifier of its class. This allows the use isA(...),
        isDirectlyA(...), isChildOf(...) and isParentOf(...).

        You can't directly create an Identifier, it's just the base-class for ClassIdentifier.
    */
    class _CoreExport Identifier
    {
        template <class T>
        friend class ClassIdentifier; // Forward declaration

        template <class T>
        friend class SubclassIdentifier; // Forward declaration

        friend class Factory; // Forward declaration

        public:
            /** @brief Sets the Factory.
             *  @param factory The factory to assign
             */
            inline void addFactory(BaseFactory* factory) { this->factory_ = factory; }

            BaseObject* fabricate();

            bool isA(const Identifier* identifier) const;
            bool isDirectlyA(const Identifier* identifier) const;
            bool isChildOf(const Identifier* identifier) const;
            bool isParentOf(const Identifier* identifier) const;

            /** @returns the name of the class the Identifier belongs to. */
            inline const std::string& getName() const { return this->name_; }

            /** @returns the parents of the class the Identifier belongs to. */
            inline const IdentifierList& getParents() const { return this->parents_; }

            /** @returns the children of the class the Identifier belongs to. */
            inline IdentifierList& getChildren() const { return *this->children_; }

            /** @returns true, if a branch of the class-hierarchy is being created, causing all new objects to store their parents. */
            inline static bool isCreatingHierarchy() { return (hierarchyCreatingCounter_s > 0); }

            /** @returns the network ID to identify a class through the network. */
            inline const unsigned int getNetworkID() const { return this->classID_; }

            /** @brief Sets the network ID to a new value. @param id The new value */
            void setNetworkID(unsigned int id);

            /** @returns the ConfigValueContainer of a variable, given by the string of its name. @param varname The name of the variable */
            inline ConfigValueContainer* getConfigValueContainer(const std::string& varname)
                { return this->configValues_[varname]; }

            /** @brief Sets the ConfigValueContainer of a variable, given by the string of its name. @param varname The name of the variablee @param container The container */
            inline void setConfigValueContainer(const std::string& varname, ConfigValueContainer* container)
                { this->configValues_[varname] = container; }

            static std::map<std::string, Identifier*>& getIdentifierMap();

        private:
            Identifier();
            Identifier(const Identifier& identifier) {} // don't copy
            virtual ~Identifier();
            void initialize(const IdentifierList* parents);

            /**
                @brief Increases the hierarchyCreatingCounter_s variable, causing all new objects to store their parents.
            */
            inline static void startCreatingHierarchy()
            {
                hierarchyCreatingCounter_s++;
                COUT(4) << "*** Increased Hierarchy-Creating-Counter to " << hierarchyCreatingCounter_s << std::endl;
            }

            /**
                @brief Decreases the hierarchyCreatingCounter_s variable, causing the objects to stop storing their parents.
            */
            inline static void stopCreatingHierarchy()
            {
                hierarchyCreatingCounter_s--;
                COUT(4) << "*** Decreased Hierarchy-Creating-Counter to " << hierarchyCreatingCounter_s << std::endl;
            }

            IdentifierList parents_;                                    //!< The Parents of the class the Identifier belongs to
            IdentifierList* children_;                                  //!< The Children of the class the Identifier belongs to

            std::string name_;                                          //!< The name of the class the Identifier belongs to

            BaseFactory* factory_;                                      //!< The Factory, able to create new objects of the given class (if available)
            bool bCreatedOneObject_;                                    //!< True if at least one object of the given type was created (used to determine the need of storing the parents)
            static int hierarchyCreatingCounter_s;                      //!< Bigger than zero if at least one Identifier stores its parents (its an int instead of a bool to avoid conflicts with multithreading)
            unsigned int classID_;                                      //!< The network ID to identify a class through the network
            std::map<std::string, ConfigValueContainer*> configValues_; //!< A map to link the string of configurable variables with their ConfigValueContainer
    };


    // ###############################
    // ###     ClassIdentifier     ###
    // ###############################
    //! The ClassIdentifier is derived from Identifier and holds all class-specific functions and variables the Identifier cannot have.
    /**
        ClassIdentifier is a Singleton, which means that only one object of a given type T exists.
        This makes it possible to store informations about a class, sharing them with all
        objects of that class without defining static variables in every class.
    */
    template <class T>
    class ClassIdentifier : public Identifier
    {
        public:
            static ClassIdentifier<T>* registerClass(const IdentifierList* parents, const std::string& name, bool bRootClass);
            static void addObject(T* object);
            static ClassIdentifier<T>* getIdentifier();
            void setName(const std::string& name);

        private:
            ClassIdentifier();
            ClassIdentifier(const ClassIdentifier<T>& identifier) {}    // don't copy
            ~ClassIdentifier() {}                                       // don't delete

            ObjectList<T>* objects_;    //!< The ObjectList, containing all objects of type T
            bool bSetName_;             //!< True if the name is set
    };

    /**
        @brief Constructor: Creates the ObjectList.
    */
    template <class T>
    ClassIdentifier<T>::ClassIdentifier()
    {
        this->objects_ = ObjectList<T>::getList();
        this->bSetName_ = false;
    }

    /**
        @brief Registers a class, which means that the name and the parents get stored.
        @param parents An IdentifierList, containing the Identifiers of all parents of the class
        @param name A string, containing exactly the name of the class
        @param bRootClass True if the class is either an Interface or the BaseObject itself
        @return The ClassIdentifier itself
    */
    template <class T>
    ClassIdentifier<T>* ClassIdentifier<T>::registerClass(const IdentifierList* parents, const std::string& name, bool bRootClass)
    {
        COUT(4) << "*** Register Class in " << name << "-Singleton." << std::endl;

        // Check if at least one object of the given type was created
        if (!getIdentifier()->bCreatedOneObject_)
        {
            // If no: We have to store the informations and initialize the Identifier
            getIdentifier()->setName(name);

            COUT(4) << "*** Register Class in " << name << "-Singleton -> Initialize Singleton." << std::endl;
            if (bRootClass)
                getIdentifier()->initialize(NULL); // If a class is derived from two interfaces, the second interface might think it's derived from the first because of the order of constructor-calls. Thats why we set parents to zero in that case.
            else
                getIdentifier()->initialize(parents);
        }

        return getIdentifier();
    }

    /**
        @brief Creates the only instance of this class for the template class T.
        @return The Identifier itself
    */
    template <class T>
    ClassIdentifier<T>* ClassIdentifier<T>::getIdentifier()
    {
        static ClassIdentifier<T> theOneAndOnlyInstance = ClassIdentifier<T>();
        static bool bIdentifierCreated = false;

        if (!bIdentifierCreated)
        {
            COUT(4) << "*** Create Identifier Singleton." << std::endl;
            bIdentifierCreated = true;
        }

        return &theOneAndOnlyInstance;
    }

    /**
        @brief Sets the name of the class.
        @param name The name
    */
    template <class T>
    void ClassIdentifier<T>::setName(const std::string& name)
    {
        // Make sure we didn't already set the name, to avoid duplicate entries in the Identifier map
        if (!this->bSetName_)
        {
            this->name_ = name;
            this->getIdentifierMap().insert(std::pair<std::string, Identifier*>(name, this));
            this->bSetName_ = true;
        }
    }

    /**
        @brief Adds an object of the given type to the ObjectList.
        @param object The object to add
    */
    template <class T>
    void ClassIdentifier<T>::addObject(T* object)
    {
        COUT(4) << "*** Added object to " << ClassIdentifier<T>::getIdentifier()->getName() << "-list." << std::endl;
        object->getMetaList().add(ClassIdentifier<T>::getIdentifier()->objects_, ClassIdentifier<T>::getIdentifier()->objects_->add(object));
    }


    // ###############################
    // ###   SubclassIdentifier    ###
    // ###############################
    //! The SubclassIdentifier acts almost like an Identifier, but has some prerequisites.
    /**
        You can only assign an Identifier that belongs to a class T (or derived) to a SubclassIdentifier<T>.
        If you assign something else, the program aborts.
        Because we know the minimal type, a dynamic_cast is done, which makes it easier to create a new object.
    */
    template <class T>
    class SubclassIdentifier
    {
        public:
            /**
                @brief Constructor: Automaticaly assigns the Identifier of the given class.
            */
            SubclassIdentifier()
            {
                this->identifier_ = ClassIdentifier<T>::getIdentifier();
            }

            /**
                @brief Overloading of the = operator: assigns the identifier and checks its type.
                @param identifier The Identifier to assign
                @return The SubclassIdentifier itself
            */
            SubclassIdentifier<T>& operator=(Identifier* identifier)
            {
                if (!identifier->isA(ClassIdentifier<T>::getIdentifier()))
                {
                    COUT(1) << "Error: Class " << identifier->getName() << " is not a " << ClassIdentifier<T>::getIdentifier()->getName() << "!" << std::endl;
                    COUT(1) << "Error: SubclassIdentifier<" << ClassIdentifier<T>::getIdentifier()->getName() << "> = Class(" << identifier->getName() << ") is forbidden." << std::endl;
                    COUT(1) << "Aborting..." << std::endl;
                    abort();
                }
                this->identifier_ = identifier;
                return *this;
            }

            /**
                @brief Overloading of the * operator: returns the assigned identifier.
                @return The assigned identifier
            */
            Identifier* operator*()
            {
                return this->identifier_;
            }

            /**
                @brief Overloading of the -> operator: returns the assigned identifier.
                @return The assigned identifier
            */
            Identifier* operator->() const
            {
                return this->identifier_;
            }

            /**
                @brief Creates a new object of the type of the assigned Identifier and dynamic_casts it to the minimal type given by T.
                @return The new object
            */
            T* fabricate()
            {
                BaseObject* newObject = this->identifier_->fabricate();

                // Check if the creation was successful
                if (newObject)
                {
                    // Do a dynamic_cast, because an object of type T is much better than of type BaseObject
                    return (T*)(newObject);
                }
                else
                {
                    // Something went terribly wrong
                    if (this->identifier_)
                    {
                        COUT(1) << "Error: Class " << this->identifier_->getName() << " is not a " << ClassIdentifier<T>::getIdentifier()->getName() << "!" << std::endl;
                        COUT(1) << "Error: Couldn't fabricate a new Object." << std::endl;
                        COUT(1) << "Aborting..." << std::endl;
                    }
                    else
                    {
                        COUT(1) << "Error: Couldn't fabricate a new Object - Identifier is undefined." << std::endl;
                        COUT(1) << "Aborting..." << std::endl;
                    }

                    abort();
                }
            }

            /** @returns the assigned identifier. */
            inline const Identifier* getIdentifier() const
                { return this->identifier_; }

            /** @returns true, if the assigned identifier is at least of the given type. @param identifier The identifier to compare with */
            inline bool isA(const Identifier* identifier) const
                { return this->identifier_->isA(identifier); }

            /** @returns true, if the assigned identifier is exactly of the given type. @param identifier The identifier to compare with */
            inline bool isDirectlyA(const Identifier* identifier) const
                { return this->identifier_->isDirectlyA(identifier); }

            /** @returns true, if the assigned identifier is a child of the given identifier. @param identifier The identifier to compare with */
            inline bool isChildOf(const Identifier* identifier) const
                { return this->identifier_->isChildOf(identifier); }

            /** @returns true, if the assigned identifier is a parent of the given identifier. @param identifier The identifier to compare with */
            inline bool isParentOf(const Identifier* identifier) const
                { return this->identifier_->isParentOf(identifier); }

        private:
            Identifier* identifier_;        //!< The assigned identifier
    };
}

#endif /* _Identifier_H__ */
