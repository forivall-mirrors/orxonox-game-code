/*!
    @file Identifier.h
    @brief Definition of the Identifier, ClassIdentifier and SubclassIdentifier classes.

    The Identifier contains all needed informations about the class it belongs to:
     - the name
     - a list with all objects
     - parents and childs
     - the factory, if available
     - the networkID that can be synchronised with the server

    Every object has a pointer to the Identifier of its class. This allows the use isA(...),
    isDirectlyA(...), isChildOf(...) and isParentOf(...).

    To create the class-hierarchy, the Identifier has some intern functions and variables.

    Every Identifier is in fact a ClassIdentifier, but they are derived from Identifier.

    SubclassIdentifier is a separated class, acting like an Identifier, but has a given class.
    You can only assign Identifiers of the given class or a derivative to a SubclassIdentifier.
*/

#ifndef _Identifier_H__
#define _Identifier_H__

#include <iostream>

#include "IdentifierList.h"
#include "ObjectList.h"
#include "Factory.h"

#define HIERARCHY_VERBOSE false


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
         - the factory, if available
         - the networkID that can be synchronised with the server

        Every object has a pointer to the Identifier of its class. This allows the use isA(...),
        isDirectlyA(...), isChildOf(...) and isParentOf(...).

        You can't directly create an Identifier, it's just the base-class for ClassIdentifier.
    */
    class Identifier
    {
        template <class T>
        friend class ClassIdentifier; // Forward declaration

        template <class T>
        friend class SubclassIdentifier; // Forward declaration

        template <class T>
        friend class ClassFactory; // Forward declaration

        public:
            /** @brief Sets the Factory. @param facotry The factory to assign */
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

            /** @returns true, if a branch of the class-hierarchy is getting created, causing all new objects to store their parents. */
            inline static bool isCreatingHierarchy() { return (hierarchyCreatingCounter_s > 0); }

            /** @returns the NetworkID to identify a class through the network. */
            inline const unsigned int getNetworkID() const { return this->classID_; }

            void setNetworkID(unsigned int id);

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
#if HIERARCHY_VERBOSE
                std::cout << "*** Increased Hierarchy-Creating-Counter to " << hierarchyCreatingCounter_s << "\n";
#endif
            }

            /**
                @brief Decreases the hierarchyCreatingCounter_s variable, causing the objects to stop storing their parents.
            */
            inline static void stopCreatingHierarchy()
            {
                hierarchyCreatingCounter_s--;
#if HIERARCHY_VERBOSE
                std::cout << "*** Decreased Hierarchy-Creating-Counter to " << hierarchyCreatingCounter_s << "\n";
#endif
            }

            IdentifierList parents_;                    //!< The Parents of the class the Identifier belongs to
            IdentifierList* children_;                  //!< The Children of the class the Identifier belongs to

            std::string name_;                          //!< The name of the class the Identifier belongs to

            BaseFactory* factory_;                      //!< The Factory, able to create new objects of the given class
            bool bCreatedOneObject_;                    //!< True if at least one object of the given type was created (used to determine the need of storing the parents)
            static int hierarchyCreatingCounter_s;      //!< Bigger than zero if at least one Identifier stores its parents (its an int instead of a bool to avoid conflicts with multithreading)
            static unsigned int classIDcounter_s;       //!< The number of unique Identifiers
            unsigned int classID_;                      //!< The networkID to identify a class through the network
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
            static ClassIdentifier<T>* getIdentifier();
            static void addObject(T* object);

        private:
            ClassIdentifier();
            ClassIdentifier(const ClassIdentifier<T>& identifier) {} // don't copy
            ~ClassIdentifier();

            static ClassIdentifier<T>* pointer_s;       //!< A pointer to the singleton-object
            ObjectList<T>* objects_;                    //!< The ObjectList, containing all objects of type T
    };

    template <class T>
    ClassIdentifier<T>* ClassIdentifier<T>::pointer_s = NULL; // Set the static member variable pointer_s to zero

    /**
        @brief Constructor: Create the ObjectList.
    */
    template <class T>
    ClassIdentifier<T>::ClassIdentifier()
    {
        this->objects_ = new ObjectList<T>;
    }

    /**
        @brief Destructor: Delete the ObjectList, set the singleton-pointer to zero.
    */
    template <class T>
    ClassIdentifier<T>::~ClassIdentifier()
    {
        delete this->objects_;
        this->pointer_s = NULL;
    }

    /**
        @brief Registers a class, which means that the name and the parents get stored.
        @param parents An IdentifierList, containing the Identifiers of all parents of the class
        @param name A string, containing exactly the name of the class
        @param bRootClass True if the class is either an Interface or BaseObject itself
        @return The ClassIdentifier itself
    */
    template <class T>
    ClassIdentifier<T>* ClassIdentifier<T>::registerClass(const IdentifierList* parents, const std::string& name, bool bRootClass)
    {
#if HIERARCHY_VERBOSE
        std::cout << "*** Register Class in " << name << "-Singleton.\n";
#endif

        // It's a singleton, so maybe we have to create it first
        if (!pointer_s)
        {
#if HIERARCHY_VERBOSE
            std::cout << "*** Register Class in " << name << "-Singleton -> Create Singleton.\n";
#endif
            pointer_s = new ClassIdentifier();
        }

        // Check if at least one object of the given type was created
        if (!pointer_s->bCreatedOneObject_)
        {
            // If no: We have to store the informations and initialize the Identifier

#if HIERARCHY_VERBOSE
            std::cout << "*** Register Class in " << name << "-Singleton -> Initialize Singleton.\n";
#endif
            pointer_s->name_ = name;
            Factory::add(name, pointer_s); // Add the Identifier to the Factory

            if (bRootClass)
                pointer_s->initialize(NULL); // If a class is derived from two interfaces, the second interface might think it's derived from the first because of the order of constructor-calls. Thats why we set parents to zero in that case.
            else
                pointer_s->initialize(parents);
        }

        return pointer_s;
    }

    /**
        @returns the Identifier itself
    */
    template <class T>
    ClassIdentifier<T>* ClassIdentifier<T>::getIdentifier()
    {
        if (!pointer_s)
        {
#if HIERARCHY_VERBOSE
            std::cout << "*** Create Singleton.\n";
#endif
            pointer_s = new ClassIdentifier();
        }

        return pointer_s;
    }

    /**
        @brief Adds an object of the given type to the ObjectList.
        @param object The object to add
    */
    template <class T>
    void ClassIdentifier<T>::addObject(T* object)
    {
#if HIERARCHY_VERBOSE
        std::cout << "*** Added object to " << ClassIdentifier<T>::getIdentifier()->getName() << "-list.\n";
#endif
        object->getMetaList().add(ClassIdentifier<T>::getIdentifier()->objects_, ClassIdentifier<T>::getIdentifier()->objects_->add(object));
    }


    // ###############################
    // ###   SubclassIdentifier    ###
    // ###############################
    //! The SubclassIdentifier acts almost like an Identifier, but has some prerequisites.
    /**
        You can only assign Identifiers that belong to a class of at least B (or derived) to a SubclassIdentifier<T>.
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
                    std::cout << "Error: Class " << identifier->getName() << " is not a " << ClassIdentifier<T>::getIdentifier()->getName() << "!\n";
                    std::cout << "Error: SubclassIdentifier<" << ClassIdentifier<T>::getIdentifier()->getName() << "> = Class(" << identifier->getName() << ") is forbidden.\n";
                    std::cout << "Aborting...\n";
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
                @brief Creates a new object of the type of the assigned identifier and dynamic_casts it to the minimal type given by the SubclassIdentifier.
                @return The new object
            */
            T* fabricate()
            {
                BaseObject* newObject = this->identifier_->fabricate();

                // Check if the creation worked
                if (newObject)
                {
                    // Do a dynamic_cast, because an object of type T is much better than of type BaseObject
                    return dynamic_cast<T*>(newObject);
                }
                else
                {
                    // Something went terribly wrong
                    if (this->identifier_)
                    {
                        std::cout << "Error: Class " << this->identifier_->getName() << " is not a " << ClassIdentifier<T>::getIdentifier()->getName() << "!\n";
                        std::cout << "Error: Couldn't fabricate a new Object.\n";
                        std::cout << "Aborting...\n";
                    }
                    else
                    {
                        std::cout << "Error: Couldn't fabricate a new Object - Identifier is undefined.\n";
                        std::cout << "Aborting...\n";
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

#endif
