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
    @brief Definition of the Identifier class, definition and implementation of the ClassIdentifier class.

    The Identifier contains all needed information about the class it belongs to:
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
*/

#ifndef _Identifier_H__
#define _Identifier_H__

#include "CorePrereqs.h"

#include <cassert>
#include <map>
#include <set>
#include <string>
#include <typeinfo>

#include "util/Debug.h"
#include "util/TypeTraits.h"
#include "MetaObjectList.h"
#include "ObjectList.h"
#include "ObjectListBase.h"
#include "Super.h"

namespace orxonox
{
    // ###############################
    // ###       Identifier        ###
    // ###############################
    //! The Identifier is used to identify the class of an object and to store information about the class.
    /**
        The Identifier contains all needed information about the class it belongs to:
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
        public:
            /** @brief Returns the name of the class the Identifier belongs to. @return The name */
            inline const std::string& getName() const { return this->name_; }
            void setName(const std::string& name);

            /** @brief Returns the network ID to identify a class through the network. @return the network ID */
            inline const uint32_t getNetworkID() const { return this->networkID_; }
            void setNetworkID(uint32_t id);

            /** @brief Returns the unique ID of the class */
            FORCEINLINE unsigned int getClassID() const { return this->classID_; }

            /** @brief Returns the list of all existing objects of this class. @return The list */
            inline ObjectListBase* getObjects() const { return this->objects_; }

            /** @brief Sets the Factory. @param factory The factory to assign */
            inline void addFactory(Factory* factory) { this->factory_ = factory; }
            /** @brief Returns true if the Identifier has a Factory. */
            inline bool hasFactory() const { return (this->factory_ != 0); }

            BaseObject* fabricate(BaseObject* creator);

            /** @brief Returns true if the class can be loaded through XML. */
            inline bool isLoadable() const { return this->bLoadable_; }
            /** @brief Set the class to be loadable through XML or not. */
            inline void setLoadable(bool bLoadable) { this->bLoadable_ = bLoadable; }

            bool isA(const Identifier* identifier) const;
            bool isExactlyA(const Identifier* identifier) const;
            bool isChildOf(const Identifier* identifier) const;
            bool isDirectChildOf(const Identifier* identifier) const;
            bool isParentOf(const Identifier* identifier) const;
            bool isDirectParentOf(const Identifier* identifier) const;


            /////////////////////////////
            ////// Class Hierarchy //////
            /////////////////////////////
            static void createClassHierarchy();

            /** @brief Returns true, if a branch of the class-hierarchy is being created, causing all new objects to store their parents. @return The status of the class-hierarchy creation */
            inline static bool isCreatingHierarchy() { return (hierarchyCreatingCounter_s > 0); }

            /** @brief Returns the parents of the class the Identifier belongs to. @return The list of all parents */
            inline const std::set<const Identifier*>& getParents() const { return this->parents_; }
            /** @brief Returns the begin-iterator of the parents-list. @return The begin-iterator */
            inline std::set<const Identifier*>::const_iterator getParentsBegin() const { return this->parents_.begin(); }
            /** @brief Returns the end-iterator of the parents-list. @return The end-iterator */
            inline std::set<const Identifier*>::const_iterator getParentsEnd() const { return this->parents_.end(); }

            /** @brief Returns the children of the class the Identifier belongs to. @return The list of all children */
            inline const std::set<const Identifier*>& getChildren() const { return this->children_; }
            /** @brief Returns the begin-iterator of the children-list. @return The begin-iterator */
            inline std::set<const Identifier*>::const_iterator getChildrenBegin() const { return this->children_.begin(); }
            /** @brief Returns the end-iterator of the children-list. @return The end-iterator */
            inline std::set<const Identifier*>::const_iterator getChildrenEnd() const { return this->children_.end(); }

            /** @brief Returns the direct parents of the class the Identifier belongs to. @return The list of all direct parents */
            inline const std::set<const Identifier*>& getDirectParents() const { return this->directParents_; }
            /** @brief Returns the begin-iterator of the direct-parents-list. @return The begin-iterator */
            inline std::set<const Identifier*>::const_iterator getDirectParentsBegin() const { return this->directParents_.begin(); }
            /** @brief Returns the end-iterator of the direct-parents-list. @return The end-iterator */
            inline std::set<const Identifier*>::const_iterator getDirectParentsEnd() const { return this->directParents_.end(); }

            /** @brief Returns the direct children the class the Identifier belongs to. @return The list of all direct children */
            inline const std::set<const Identifier*>& getDirectChildren() const { return this->directChildren_; }
            /** @brief Returns the begin-iterator of the direct-children-list. @return The begin-iterator */
            inline std::set<const Identifier*>::const_iterator getDirectChildrenBegin() const { return this->directChildren_.begin(); }
            /** @brief Returns the end-iterator of the direct-children-list. @return The end-iterator */
            inline std::set<const Identifier*>::const_iterator getDirectChildrenEnd() const { return this->directChildren_.end(); }


            //////////////////////////
            ///// Identifier Map /////
            //////////////////////////
            static void destroyAllIdentifiers();

            static Identifier* getIdentifierByString(const std::string& name);
            static Identifier* getIdentifierByLowercaseString(const std::string& name);
            static Identifier* getIdentifierByID(uint32_t id);

            static void clearNetworkIDs();

            /** @brief Returns the map that stores all Identifiers with their names. @return The map */
            static inline const std::map<std::string, Identifier*>& getStringIdentifierMap() { return Identifier::getStringIdentifierMapIntern(); }
            /** @brief Returns a const_iterator to the beginning of the map that stores all Identifiers with their names. @return The const_iterator */
            static inline std::map<std::string, Identifier*>::const_iterator getStringIdentifierMapBegin() { return Identifier::getStringIdentifierMap().begin(); }
            /** @brief Returns a const_iterator to the end of the map that stores all Identifiers with their names. @return The const_iterator */
            static inline std::map<std::string, Identifier*>::const_iterator getStringIdentifierMapEnd() { return Identifier::getStringIdentifierMap().end(); }

            /** @brief Returns the map that stores all Identifiers with their names in lowercase. @return The map */
            static inline const std::map<std::string, Identifier*>& getLowercaseStringIdentifierMap() { return Identifier::getLowercaseStringIdentifierMapIntern(); }
            /** @brief Returns a const_iterator to the beginning of the map that stores all Identifiers with their names in lowercase. @return The const_iterator */
            static inline std::map<std::string, Identifier*>::const_iterator getLowercaseStringIdentifierMapBegin() { return Identifier::getLowercaseStringIdentifierMap().begin(); }
            /** @brief Returns a const_iterator to the end of the map that stores all Identifiers with their names in lowercase. @return The const_iterator */
            static inline std::map<std::string, Identifier*>::const_iterator getLowercaseStringIdentifierMapEnd() { return Identifier::getLowercaseStringIdentifierMap().end(); }

            /** @brief Returns the map that stores all Identifiers with their IDs. @return The map */
            static inline const std::map<uint32_t, Identifier*>& getIDIdentifierMap() { return Identifier::getIDIdentifierMapIntern(); }
            /** @brief Returns a const_iterator to the beginning of the map that stores all Identifiers with their IDs. @return The const_iterator */
            static inline std::map<uint32_t, Identifier*>::const_iterator getIDIdentifierMapBegin() { return Identifier::getIDIdentifierMap().begin(); }
            /** @brief Returns a const_iterator to the end of the map that stores all Identifiers with their IDs. @return The const_iterator */
            static inline std::map<uint32_t, Identifier*>::const_iterator getIDIdentifierMapEnd() { return Identifier::getIDIdentifierMap().end(); }


            /////////////////////////
            ///// Config Values /////
            /////////////////////////
            virtual void updateConfigValues(bool updateChildren = true) const = 0;

            /** @brief Returns true if this class has at least one config value. @return True if this class has at least one config value */
            inline bool hasConfigValues() const { return this->bHasConfigValues_; }

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

            void addConfigValueContainer(const std::string& varname, ConfigValueContainer* container);
            ConfigValueContainer* getConfigValueContainer(const std::string& varname);
            ConfigValueContainer* getLowercaseConfigValueContainer(const std::string& varname);


            ////////////////////////////
            ///// Console Commands /////
            ////////////////////////////
            /** @brief Returns true if this class has at least one console command. @return True if this class has at least one console command */
            inline bool hasConsoleCommands() const { return this->bHasConsoleCommands_; }

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

            ConsoleCommand& addConsoleCommand(ConsoleCommand* command, bool bCreateShortcut);
            ConsoleCommand* getConsoleCommand(const std::string& name) const;
            ConsoleCommand* getLowercaseConsoleCommand(const std::string& name) const;


            ///////////////////
            ///// XMLPort /////
            ///////////////////
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

            void addXMLPortParamContainer(const std::string& paramname, XMLPortParamContainer* container);
            XMLPortParamContainer* getXMLPortParamContainer(const std::string& paramname);

            void addXMLPortObjectContainer(const std::string& sectionname, XMLPortObjectContainer* container);
            XMLPortObjectContainer* getXMLPortObjectContainer(const std::string& sectionname);


        protected:
            Identifier();
            Identifier(const Identifier& identifier); // don't copy
            virtual ~Identifier();

            static Identifier* getIdentifierSingleton(const std::string& name, Identifier* proposal);
            virtual void createSuperFunctionCaller() const = 0;

            void initializeClassHierarchy(std::set<const Identifier*>* parents, bool bRootClass);

            /** @brief Returns the map that stores all Identifiers with their names. @return The map */
            static std::map<std::string, Identifier*>& getStringIdentifierMapIntern();
            /** @brief Returns the map that stores all Identifiers with their names in lowercase. @return The map */
            static std::map<std::string, Identifier*>& getLowercaseStringIdentifierMapIntern();
            /** @brief Returns the map that stores all Identifiers with their network IDs. @return The map */
            static std::map<uint32_t, Identifier*>& getIDIdentifierMapIntern();

            /** @brief Returns the children of the class the Identifier belongs to. @return The list of all children */
            inline std::set<const Identifier*>& getChildrenIntern() const { return this->children_; }
            /** @brief Returns the direct children of the class the Identifier belongs to. @return The list of all direct children */
            inline std::set<const Identifier*>& getDirectChildrenIntern() const { return this->directChildren_; }

            ObjectListBase* objects_;                                      //!< The list of all objects of this class

        private:
            /** @brief Increases the hierarchyCreatingCounter_s variable, causing all new objects to store their parents. */
            inline static void startCreatingHierarchy() { hierarchyCreatingCounter_s++; }
            /** @brief Decreases the hierarchyCreatingCounter_s variable, causing the objects to stop storing their parents. */
            inline static void stopCreatingHierarchy()  { hierarchyCreatingCounter_s--; }

            static std::map<std::string, Identifier*>& getTypeIDIdentifierMap();

            void initialize(std::set<const Identifier*>* parents);

            std::set<const Identifier*> parents_;                          //!< The parents of the class the Identifier belongs to
            mutable std::set<const Identifier*> children_;                 //!< The children of the class the Identifier belongs to

            std::set<const Identifier*> directParents_;                    //!< The direct parents of the class the Identifier belongs to
            mutable std::set<const Identifier*> directChildren_;           //!< The direct children of the class the Identifier belongs to

            bool bCreatedOneObject_;                                       //!< True if at least one object of the given type was created (used to determine the need of storing the parents)
            bool bSetName_;                                                //!< True if the name is set
            bool bLoadable_;                                               //!< False = it's not permitted to load the object through XML
            std::string name_;                                             //!< The name of the class the Identifier belongs to
            Factory* factory_;                                             //!< The Factory, able to create new objects of the given class (if available)
            static int hierarchyCreatingCounter_s;                         //!< Bigger than zero if at least one Identifier stores its parents (its an int instead of a bool to avoid conflicts with multithreading)
            uint32_t networkID_;                                           //!< The network ID to identify a class through the network
            const unsigned int classID_;                                   //!< Uniquely identifies a class (might not be the same as the networkID_)
            static unsigned int classIDCounter_s;                          //!< Static counter for the unique classIDs

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
        This makes it possible to store information about a class, sharing them with all
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
            static ClassIdentifier<T>* getIdentifier();
            static ClassIdentifier<T>* getIdentifier(const std::string& name);

            bool initialiseObject(T* object, const std::string& className, bool bRootClass);

            void updateConfigValues(bool updateChildren = true) const;

        private:
            static void initialiseIdentifier();
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
    ClassIdentifier<T>* ClassIdentifier<T>::classIdentifier_s = 0;

    /**
        @brief Returns the only instance of this class.
        @return The unique Identifier
    */
    template <class T>
    inline ClassIdentifier<T>* ClassIdentifier<T>::getIdentifier()
    {
        // check if the Identifier already exists
        if (!ClassIdentifier<T>::classIdentifier_s)
            ClassIdentifier<T>::initialiseIdentifier();

        return ClassIdentifier<T>::classIdentifier_s;
    }

    /**
        @brief Does the same as getIdentifier() but sets the name if this wasn't done yet.
        @param name The name of this Identifier
        @return The Identifier
    */
    template <class T>
    inline ClassIdentifier<T>* ClassIdentifier<T>::getIdentifier(const std::string& name)
    {
        ClassIdentifier<T>* identifier = ClassIdentifier<T>::getIdentifier();
        identifier->setName(name);
        return identifier;
    }

    /**
        @brief Assigns the static field for the identifier singleton.
    */
    template <class T>
    void ClassIdentifier<T>::initialiseIdentifier()
    {
        // Get the name of the class
        const std::string& name = typeid(T).name();

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

    /**
        @brief Adds an object of the given type to the ObjectList.
        @param object The object to add
    */
    template <class T>
    bool ClassIdentifier<T>::initialiseObject(T* object, const std::string& className, bool bRootClass)
    {
        if (bRootClass)
            COUT(5) << "*** Register Root-Object: " << className << std::endl;
        else
            COUT(5) << "*** Register Object: " << className << std::endl;

        object->identifier_ = this;
        if (Identifier::isCreatingHierarchy())
        {
            if (bRootClass && !object->parents_)
                object->parents_ = new std::set<const Identifier*>();

            if (object->parents_)
            {
                this->initializeClassHierarchy(object->parents_, bRootClass);
                object->parents_->insert(object->parents_->end(), this);
            }

            object->setConfigValues();
            return true;
        }
        else
        {
            COUT(5) << "*** ClassIdentifier: Added object to " << this->getName() << "-list." << std::endl;
            object->metaList_->add(this->objects_, this->objects_->add(new ObjectListElement<T>(object)));

            // Add pointer of type T to the map in the OrxonoxClass instance that enables "dynamic_casts"
            object->objectPointers_.push_back(std::make_pair(this->getClassID(), static_cast<void*>(object)));
            return false;
        }
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
    // ###      orxonox_cast       ###
    // ###############################
    /**
    @brief
        Casts on object of type OrxonoxClass to any derived type that is
        registered in the class hierarchy.
    @return
        Returns NULL if the cast is not possible
    @note
        In case of NULL return (and using MSVC), a dynamic_cast might still be possible if
        a class forgot to register its objects.
        Also note that the function is implemented differently for GCC/MSVC.
    */
    template <class T, class U>
    FORCEINLINE T orxonox_cast(U source)
    {
#ifdef ORXONOX_COMPILER_MSVC
        typedef Loki::TypeTraits<typename Loki::TypeTraits<T>::PointeeType>::NonConstType ClassType;
        if (source != NULL)
            return source->template getDerivedPointer<ClassType>(ClassIdentifier<ClassType>::getIdentifier()->getClassID());
        else
            return NULL;
#else
        return dynamic_cast<T>(source);
#endif
    }
}

#endif /* _Identifier_H__ */
