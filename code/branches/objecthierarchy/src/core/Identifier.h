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
    @file Identifier.h
    @brief Definition of the Identifier, ClassIdentifier and SubclassIdentifier classes, implementation of the ClassIdentifier and SubclassIdentifier classes.

    The Identifier contains all needed informations about the class it belongs to:
     - the name
     - a list with all objects
     - parents and children
     - the factory (if available)
     - the networkID that can be synchronised with the server
     - all configurable variables (if available)

    Every object has a pointer to the Identifier of its class. This allows the use isA(...),
    isExactlyA(...), isChildOf(...) and isParentOf(...).

    To create the class-hierarchy, the Identifier has some intern functions and variables.

    Every Identifier is in fact a ClassIdentifier, but they are derived from Identifier.

    SubclassIdentifier is a separated class, acting like an Identifier, but has a given class.
    You can only assign Identifiers of exactly the given class or of a derivative to a SubclassIdentifier.
*/

#ifndef _Identifier_H__
#define _Identifier_H__

#include "CorePrereqs.h"

#include <set>
#include <map>
#include <string>
#include <utility>
#include <typeinfo>
#include <stdlib.h>
#include <cassert>

#include "MetaObjectList.h"
#include "Iterator.h"
#include "Super.h"
#include "util/Debug.h"
#include "util/String.h"

namespace orxonox
{
    // ###############################
    // ###       Identifier        ###
    // ###############################
    //! The Identifier is used to identify the class of an object and to store informations about the class.
    /**
        The Identifier contains all needed informations about the class it belongs to:
         - the name
         - a list with all objects
         - parents and children
         - the factory (if available)
         - the networkID that can be synchronised with the server
         - all configurable variables (if available)

        Every object has a pointer to the Identifier of its class. This allows the use isA(...),
        isExactlyA(...), isChildOf(...) and isParentOf(...).

        You can't directly create an Identifier, it's just the base-class for ClassIdentifier.
    */
    class _CoreExport Identifier
    {
        template <class T>
        friend class SubclassIdentifier;

        friend class Factory;

        public:
            /** @brief Sets the Factory. @param factory The factory to assign */
            inline void addFactory(BaseFactory* factory) { this->factory_ = factory; }

            BaseObject* fabricate(BaseObject* creator);
            bool isA(const Identifier* identifier) const;
            bool isExactlyA(const Identifier* identifier) const;
            bool isChildOf(const Identifier* identifier) const;
            bool isDirectChildOf(const Identifier* identifier) const;
            bool isParentOf(const Identifier* identifier) const;
            bool isDirectParentOf(const Identifier* identifier) const;

            /** @brief Returns true if the class can be loaded through XML. */
            inline bool isLoadable() const { return this->bLoadable_; }
            /** @brief Set the class to be loadable through XML or not. */
            inline void setLoadable(bool bLoadable) { this->bLoadable_ = bLoadable; }

            /** @brief Returns the list of all existing objects of this class. @return The list */
            inline ObjectListBase* getObjects() const
                { return this->objects_; }

            /** @brief Returns the name of the class the Identifier belongs to. @return The name */
            inline const std::string& getName() const { return this->name_; }
            void setName(const std::string& name);

            virtual void updateConfigValues(bool updateChildren = true) const = 0;

            /** @brief Returns the parents of the class the Identifier belongs to. @return The list of all parents */
            inline const std::set<const Identifier*>& getParents() const { return this->parents_; }
            /** @brief Returns the begin-iterator of the parents-list. @return The begin-iterator */
            inline std::set<const Identifier*>::const_iterator getParentsBegin() const { return this->parents_.begin(); }
            /** @brief Returns the end-iterator of the parents-list. @return The end-iterator */
            inline std::set<const Identifier*>::const_iterator getParentsEnd() const { return this->parents_.end(); }

            /** @brief Returns the children of the class the Identifier belongs to. @return The list of all children */
            inline const std::set<const Identifier*>& getChildren() const { return (*this->children_); }
            /** @brief Returns the begin-iterator of the children-list. @return The begin-iterator */
            inline std::set<const Identifier*>::const_iterator getChildrenBegin() const { return this->children_->begin(); }
            /** @brief Returns the end-iterator of the children-list. @return The end-iterator */
            inline std::set<const Identifier*>::const_iterator getChildrenEnd() const { return this->children_->end(); }

            /** @brief Returns the direct parents of the class the Identifier belongs to. @return The list of all direct parents */
            inline const std::set<const Identifier*>& getDirectParents() const { return this->directParents_; }
            /** @brief Returns the begin-iterator of the direct-parents-list. @return The begin-iterator */
            inline std::set<const Identifier*>::const_iterator getDirectParentsBegin() const { return this->directParents_.begin(); }
            /** @brief Returns the end-iterator of the direct-parents-list. @return The end-iterator */
            inline std::set<const Identifier*>::const_iterator getDirectParentsEnd() const { return this->directParents_.end(); }

            /** @brief Returns the direct children the class the Identifier belongs to. @return The list of all direct children */
            inline const std::set<const Identifier*>& getDirectChildren() const { return (*this->directChildren_); }
            /** @brief Returns the begin-iterator of the direct-children-list. @return The begin-iterator */
            inline std::set<const Identifier*>::const_iterator getDirectChildrenBegin() const { return this->directChildren_->begin(); }
            /** @brief Returns the end-iterator of the direct-children-list. @return The end-iterator */
            inline std::set<const Identifier*>::const_iterator getDirectChildrenEnd() const { return this->directChildren_->end(); }


            /** @brief Returns the map that stores all Identifiers. @return The map */
            static inline const std::map<std::string, Identifier*>& getIdentifierMap() { return Identifier::getIdentifierMapIntern(); }
            /** @brief Returns a const_iterator to the beginning of the map that stores all Identifiers. @return The const_iterator */
            static inline std::map<std::string, Identifier*>::const_iterator getIdentifierMapBegin() { return Identifier::getIdentifierMap().begin(); }
            /** @brief Returns a const_iterator to the end of the map that stores all Identifiers. @return The const_iterator */
            static inline std::map<std::string, Identifier*>::const_iterator getIdentifierMapEnd() { return Identifier::getIdentifierMap().end(); }

            /** @brief Returns the map that stores all Identifiers with their names in lowercase. @return The map */
            static inline const std::map<std::string, Identifier*>& getLowercaseIdentifierMap() { return Identifier::getLowercaseIdentifierMapIntern(); }
            /** @brief Returns a const_iterator to the beginning of the map that stores all Identifiers with their names in lowercase. @return The const_iterator */
            static inline std::map<std::string, Identifier*>::const_iterator getLowercaseIdentifierMapBegin() { return Identifier::getLowercaseIdentifierMap().begin(); }
            /** @brief Returns a const_iterator to the end of the map that stores all Identifiers with their names in lowercase. @return The const_iterator */
            static inline std::map<std::string, Identifier*>::const_iterator getLowercaseIdentifierMapEnd() { return Identifier::getLowercaseIdentifierMap().end(); }


            /** @brief Returns the map that stores all config values. @return The const_iterator */
            inline const std::map<std::string, ConfigValueContainer*>& getConfigValueMap() const { return this->configValues_; }
            /** @brief Returns a const_iterator to the beginning of the map that stores all config values. @return The const_iterator */
            inline std::map<std::string, ConfigValueContainer*>::const_iterator getConfigValueMapBegin() const { return this->configValues_.begin(); }
            /** @brief Returns a const_iterator to the end of the map that stores all config values. @return The const_iterator */
            inline std::map<std::string, ConfigValueContainer*>::const_iterator getConfigValueMapEnd() const { return this->configValues_.end(); }

            /** @brief Returns the map that stores all config values with their names in lowercase. @return The const_iterator */
            inline const std::map<std::string, ConfigValueContainer*>& getLowercaseConfigValueMap() const { return this->configValues_LC_; }
            /** @brief Returns a const_iterator to the beginning of the map that stores all config values with their names in lowercase. @return The const_iterator */
            inline std::map<std::string, ConfigValueContainer*>::const_iterator getLowercaseConfigValueMapBegin() const { return this->configValues_LC_.begin(); }
            /** @brief Returns a const_iterator to the end of the map that stores all config values with their names in lowercase. @return The const_iterator */
            inline std::map<std::string, ConfigValueContainer*>::const_iterator getLowercaseConfigValueMapEnd() const { return this->configValues_LC_.end(); }


            /** @brief Returns the map that stores all console commands. @return The const_iterator */
            inline const std::map<std::string, ConsoleCommand*>& getConsoleCommandMap() const { return this->consoleCommands_; }
            /** @brief Returns a const_iterator to the beginning of the map that stores all console commands. @return The const_iterator */
            inline std::map<std::string, ConsoleCommand*>::const_iterator getConsoleCommandMapBegin() const { return this->consoleCommands_.begin(); }
            /** @brief Returns a const_iterator to the end of the map that stores all console commands. @return The const_iterator */
            inline std::map<std::string, ConsoleCommand*>::const_iterator getConsoleCommandMapEnd() const { return this->consoleCommands_.end(); }

            /** @brief Returns the map that stores all console commands with their names in lowercase. @return The const_iterator */
            inline const std::map<std::string, ConsoleCommand*>& getLowercaseConsoleCommandMap() const { return this->consoleCommands_LC_; }
            /** @brief Returns a const_iterator to the beginning of the map that stores all console commands with their names in lowercase. @return The const_iterator */
            inline std::map<std::string, ConsoleCommand*>::const_iterator getLowercaseConsoleCommandMapBegin() const { return this->consoleCommands_LC_.begin(); }
            /** @brief Returns a const_iterator to the end of the map that stores all console commands with their names in lowercase. @return The const_iterator */
            inline std::map<std::string, ConsoleCommand*>::const_iterator getLowercaseConsoleCommandMapEnd() const { return this->consoleCommands_LC_.end(); }

            /** @brief Returns the map that stores all XMLPort params. @return The const_iterator */
            inline const std::map<std::string, XMLPortParamContainer*>& getXMLPortParamMap() const { return this->xmlportParamContainers_; }
            /** @brief Returns a const_iterator to the beginning of the map that stores all XMLPort params. @return The const_iterator */
            inline std::map<std::string, XMLPortParamContainer*>::const_iterator getXMLPortParamMapBegin() const { return this->xmlportParamContainers_.begin(); }
            /** @brief Returns a const_iterator to the end of the map that stores all XMLPort params. @return The const_iterator */
            inline std::map<std::string, XMLPortParamContainer*>::const_iterator getXMLPortParamMapEnd() const { return this->xmlportParamContainers_.end(); }

            /** @brief Returns the map that stores all XMLPort objects. @return The const_iterator */
            inline const std::map<std::string, XMLPortObjectContainer*>& getXMLPortObjectMap() const { return this->xmlportObjectContainers_; }
            /** @brief Returns a const_iterator to the beginning of the map that stores all XMLPort objects. @return The const_iterator */
            inline std::map<std::string, XMLPortObjectContainer*>::const_iterator getXMLPortObjectMapBegin() const { return this->xmlportObjectContainers_.begin(); }
            /** @brief Returns a const_iterator to the end of the map that stores all XMLPort objects. @return The const_iterator */
            inline std::map<std::string, XMLPortObjectContainer*>::const_iterator getXMLPortObjectMapEnd() const { return this->xmlportObjectContainers_.end(); }

            /** @brief Returns true if this class has at least one config value. @return True if this class has at least one config value */
            inline bool hasConfigValues() const { return this->bHasConfigValues_; }
            /** @brief Returns true if this class has at least one console command. @return True if this class has at least one console command */
            inline bool hasConsoleCommands() const { return this->bHasConsoleCommands_; }

            /** @brief Returns true, if a branch of the class-hierarchy is being created, causing all new objects to store their parents. @return The status of the class-hierarchy creation */
            inline static bool isCreatingHierarchy() { return (hierarchyCreatingCounter_s > 0); }

            /** @brief Returns the network ID to identify a class through the network. @return the network ID */
            inline const unsigned int getNetworkID() const { return this->classID_; }

            /** @brief Sets the network ID to a new value. @param id The new value */
            void setNetworkID(unsigned int id);

            void addConfigValueContainer(const std::string& varname, ConfigValueContainer* container);
            ConfigValueContainer* getConfigValueContainer(const std::string& varname);
            ConfigValueContainer* getLowercaseConfigValueContainer(const std::string& varname);

            void addXMLPortParamContainer(const std::string& paramname, XMLPortParamContainer* container);
            XMLPortParamContainer* getXMLPortParamContainer(const std::string& paramname);

            void addXMLPortObjectContainer(const std::string& sectionname, XMLPortObjectContainer* container);
            XMLPortObjectContainer* getXMLPortObjectContainer(const std::string& sectionname);

            ConsoleCommand& addConsoleCommand(ConsoleCommand* command, bool bCreateShortcut);
            ConsoleCommand* getConsoleCommand(const std::string& name) const;
            ConsoleCommand* getLowercaseConsoleCommand(const std::string& name) const;

            void initializeClassHierarchy(std::set<const Identifier*>* parents, bool bRootClass);

        protected:
            Identifier();
            Identifier(const Identifier& identifier); // don't copy
            virtual ~Identifier();

            static Identifier* getIdentifierSingleton(const std::string& name, Identifier* proposal);
            virtual void createSuperFunctionCaller() const = 0;

            /** @brief Returns the map that stores all Identifiers. @return The map */
            static std::map<std::string, Identifier*>& getIdentifierMapIntern();
            /** @brief Returns the map that stores all Identifiers with their names in lowercase. @return The map */
            static std::map<std::string, Identifier*>& getLowercaseIdentifierMapIntern();

            /** @brief Returns the children of the class the Identifier belongs to. @return The list of all children */
            inline std::set<const Identifier*>& getChildrenIntern() const { return (*this->children_); }
            /** @brief Returns the direct children of the class the Identifier belongs to. @return The list of all direct children */
            inline std::set<const Identifier*>& getDirectChildrenIntern() const { return (*this->directChildren_); }

            ObjectListBase* objects_;                                      //!< The list of all objects of this class

        private:
            /**
                @brief Increases the hierarchyCreatingCounter_s variable, causing all new objects to store their parents.
            */
            inline static void startCreatingHierarchy()
            {
                hierarchyCreatingCounter_s++;
                COUT(4) << "*** Identifier: Increased Hierarchy-Creating-Counter to " << hierarchyCreatingCounter_s << std::endl;
            }

            /**
                @brief Decreases the hierarchyCreatingCounter_s variable, causing the objects to stop storing their parents.
            */
            inline static void stopCreatingHierarchy()
            {
                hierarchyCreatingCounter_s--;
                COUT(4) << "*** Identifier: Decreased Hierarchy-Creating-Counter to " << hierarchyCreatingCounter_s << std::endl;
            }

            void initialize(std::set<const Identifier*>* parents);

            static void destroyAllIdentifiers();

            std::set<const Identifier*> parents_;                          //!< The parents of the class the Identifier belongs to
            std::set<const Identifier*>* children_;                        //!< The children of the class the Identifier belongs to

            std::set<const Identifier*> directParents_;                    //!< The direct parents of the class the Identifier belongs to
            std::set<const Identifier*>* directChildren_;                  //!< The direct children of the class the Identifier belongs to

            bool bCreatedOneObject_;                                       //!< True if at least one object of the given type was created (used to determine the need of storing the parents)
            bool bSetName_;                                                //!< True if the name is set
            bool bLoadable_;                                               //!< False = it's not permitted to load the object through XML
            std::string name_;                                             //!< The name of the class the Identifier belongs to
            BaseFactory* factory_;                                         //!< The Factory, able to create new objects of the given class (if available)
            static int hierarchyCreatingCounter_s;                         //!< Bigger than zero if at least one Identifier stores its parents (its an int instead of a bool to avoid conflicts with multithreading)
            unsigned int classID_;                                         //!< The network ID to identify a class through the network

            bool bHasConfigValues_;                                        //!< True if this class has at least one assigned config value
            std::map<std::string, ConfigValueContainer*> configValues_;    //!< A map to link the string of configurable variables with their ConfigValueContainer
            std::map<std::string, ConfigValueContainer*> configValues_LC_; //!< A map to link the string of configurable variables with their ConfigValueContainer

            bool bHasConsoleCommands_;                                     //!< True if this class has at least one assigned console command
            std::map<std::string, ConsoleCommand*> consoleCommands_;       //!< All console commands of this class
            std::map<std::string, ConsoleCommand*> consoleCommands_LC_;    //!< All console commands of this class with their names in lowercase

            std::map<std::string, XMLPortParamContainer*> xmlportParamContainers_;     //!< All loadable parameters
            std::map<std::string, XMLPortObjectContainer*> xmlportObjectContainers_;   //!< All attachable objects
    };

    _CoreExport std::ostream& operator<<(std::ostream& out, const std::set<const Identifier*>& list);


    // ###############################
    // ###     ClassIdentifier     ###
    // ###############################
    //! The ClassIdentifier is derived from Identifier and holds all class-specific functions and variables the Identifier cannot have.
    /**
        ClassIdentifier is a Singleton, which means that only one object of a given type T exists.
        This makes it possible to store informations about a class, sharing them with all
        objects of that class without defining static variables in every class.

        To be really sure that not more than exactly one object exists (even with libraries),
        ClassIdentifiers are stored in the Identifier Singleton.
    */
    template <class T>
    class ClassIdentifier : public Identifier
    {
        #define SUPER_INTRUSIVE_DECLARATION_INCLUDE
        #include "Super.h"

        public:
            static ClassIdentifier<T> *getIdentifier();
            static ClassIdentifier<T> *getIdentifier(const std::string& name);
            void addObject(T* object);
            static bool isFirstCall();

            void updateConfigValues(bool updateChildren = true) const;

        private:
            ClassIdentifier(const ClassIdentifier<T>& identifier) {}    // don't copy
            ClassIdentifier()
            {
                SuperFunctionInitialization<0, T>::initialize(this);
            }
            ~ClassIdentifier()
            {
                SuperFunctionDestruction<0, T>::destroy(this);
            }

            static ClassIdentifier<T>* classIdentifier_s;
    };

    template <class T>
    ClassIdentifier<T>* ClassIdentifier<T>::classIdentifier_s;

    /**
        @brief Returns true if the function gets called the first time, false otherwise.
        @return True if this function got called the first time.
    */
    template <class T>
    bool ClassIdentifier<T>::isFirstCall()
    {
        static bool bFirstCall = true;

        if (bFirstCall)
        {
            bFirstCall = false;
            return true;
        }
        else
        {
            return false;
        }
    }

    /**
        @brief Returns the only instance of this class.
        @return The unique Identifier
    */
    template <class T>
    ClassIdentifier<T>* ClassIdentifier<T>::getIdentifier()
    {
        // check if the static field has already been filled
        if (ClassIdentifier<T>::isFirstCall())
        {
            // Get the name of the class
            std::string name = typeid(T).name();

            // create a new identifier anyway. Will be deleted in Identifier::getIdentifier if not used.
            ClassIdentifier<T>* proposal = new ClassIdentifier<T>();

            // Get the entry from the map
            ClassIdentifier<T>::classIdentifier_s = (ClassIdentifier<T>*)Identifier::getIdentifierSingleton(name, proposal);

            if (ClassIdentifier<T>::classIdentifier_s == proposal)
            {
                COUT(4) << "*** Identifier: Requested Identifier for " << name << " was not yet existing and got created." << std::endl;
            }
            else
            {
                COUT(4) << "*** Identifier: Requested Identifier for " << name << " was already existing and got assigned." << std::endl;
            }
        }

        // Finally return the unique ClassIdentifier
        return ClassIdentifier<T>::classIdentifier_s;
    }

    /**
        @brief Does the same as getIdentifier() but sets the name if this wasn't done yet.
        @param name The name of this Identifier
        @return The Identifier
    */
    template <class T>
    ClassIdentifier<T>* ClassIdentifier<T>::getIdentifier(const std::string& name)
    {
        ClassIdentifier<T>* identifier = ClassIdentifier<T>::getIdentifier();
        identifier->setName(name);
        return identifier;
    }

    /**
        @brief Adds an object of the given type to the ObjectList.
        @param object The object to add
    */
    template <class T>
    void ClassIdentifier<T>::addObject(T* object)
    {
        COUT(5) << "*** ClassIdentifier: Added object to " << this->getName() << "-list." << std::endl;
        object->getMetaList().add(this->objects_, this->objects_->add(new ObjectListElement<T>(object)));
    }

    /**
        @brief Updates the config-values of all existing objects of this class by calling their setConfigValues() function.
    */
    template <class T>
    void ClassIdentifier<T>::updateConfigValues(bool updateChildren) const
    {
        if (!this->hasConfigValues())
            return;

        for (ObjectListIterator<T> it = ObjectList<T>::begin(); it; ++it)
            it->setConfigValues();

        if (updateChildren)
            for (std::set<const Identifier*>::const_iterator it = this->getChildrenBegin(); it != this->getChildrenEnd(); ++it)
                (*it)->updateConfigValues(false);
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
                @brief Copyconstructor: Assigns the given Identifier.
                @param identifier The Identifier
            */
            SubclassIdentifier(Identifier* identifier)
            {
                this->operator=(identifier);
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
                    COUT(1) << "An error occurred in SubclassIdentifier (Identifier.h):" << std::endl;
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
            */
            inline Identifier* operator*() const
            {
                return this->identifier_;
            }

            /**
                @brief Overloading of the -> operator: returns the assigned identifier.
            */
            inline Identifier* operator->() const
            {
                return this->identifier_;
            }

            /**
                @brief Returns the assigned identifier. This allows you to assign a SubclassIdentifier to a normal Identifier*.
            */
            inline operator Identifier*() const
            {
                return this->identifier_;
            }

            /**
                @brief Creates a new object of the type of the assigned Identifier and dynamic_casts it to the minimal type given by T.
                @return The new object
            */
            T* fabricate(BaseObject* creator) const
            {
                BaseObject* newObject = this->identifier_->fabricate(creator);

                // Check if the creation was successful
                if (newObject)
                {
                    return dynamic_cast<T*>(newObject);
                }
                else
                {
                    // Something went terribly wrong
                    if (this->identifier_)
                    {
                        COUT(1) << "An error occurred in SubclassIdentifier (Identifier.h):" << std::endl;
                        COUT(1) << "Error: Class " << this->identifier_->getName() << " is not a " << ClassIdentifier<T>::getIdentifier()->getName() << "!" << std::endl;
                        COUT(1) << "Error: Couldn't fabricate a new Object." << std::endl;
                        COUT(1) << "Aborting..." << std::endl;
                    }
                    else
                    {
                        COUT(1) << "An error occurred in SubclassIdentifier (Identifier.h):" << std::endl;
                        COUT(1) << "Error: Couldn't fabricate a new Object - Identifier is undefined." << std::endl;
                        COUT(1) << "Aborting..." << std::endl;
                    }

                    assert(false);
                    return 0;
                }
            }

            /** @brief Returns the assigned identifier. @return The identifier */
            inline Identifier* getIdentifier() const
                { return this->identifier_; }

//            /** @brief Returns true, if the assigned identifier is at least of the given type. @param identifier The identifier to compare with */
//            inline bool isA(const Identifier* identifier) const
//                { return this->identifier_->isA(identifier); }
//
//            /** @brief Returns true, if the assigned identifier is exactly of the given type. @param identifier The identifier to compare with */
//            inline bool isExactlyA(const Identifier* identifier) const
//                { return this->identifier_->isExactlyA(identifier); }
//
//            /** @brief Returns true, if the assigned identifier is a child of the given identifier. @param identifier The identifier to compare with */
//            inline bool isChildOf(const Identifier* identifier) const
//                { return this->identifier_->isChildOf(identifier); }
//
//            /** @brief Returns true, if the assigned identifier is a direct child of the given identifier. @param identifier The identifier to compare with */
//            inline bool isDirectChildOf(const Identifier* identifier) const
//                { return this->identifier_->isDirectChildOf(identifier); }
//
//            /** @brief Returns true, if the assigned identifier is a parent of the given identifier. @param identifier The identifier to compare with */
//            inline bool isParentOf(const Identifier* identifier) const
//                { return this->identifier_->isParentOf(identifier); }
//
//            /** @brief Returns true, if the assigned identifier is a direct parent of the given identifier. @param identifier The identifier to compare with */
//            inline bool isDirectParentOf(const Identifier* identifier) const
//                { return this->identifier_->isDirectParentOf(identifier); }

        private:
            Identifier* identifier_;            //!< The assigned identifier
    };
}

#endif /* _Identifier_H__ */
