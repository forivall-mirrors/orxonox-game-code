#ifndef _Identifier_H__
#define _Identifier_H__

#include <iostream>

#include "IdentifierList.h"
#include "ObjectList.h"
//#include "OrxonoxClass.h"
#include "Factory.h"

// DONE AND TESTED:
// - build class hierarchy
// - isA, isChildOf, ...
// - insert into class-lists
// - ClassIdentifier
// - BaseIdentifier
// - Factory

// IN WORK:

// TO DO:
// - iterate through lists

namespace orxonox
{
    class BaseObject;

    // ##### Identifier #####
    class Identifier
    {
        template <class T>
        friend class ClassIdentifier;

        template <class T>
        friend class BaseIdentifier;

        template <class T>
        friend class Iterator;

        public:
            void addObject(OrxonoxClass* object);
            void removeObject(OrxonoxClass* object);

            virtual BaseObject* fabricate() {};

            bool isA(Identifier* identifier);
            bool isDirectlyA(Identifier* identifier);
            bool isChildOf(Identifier* identifier);
            bool isDirectChildOf(Identifier* identifier);
            bool isParentOf(Identifier* identifier);
            bool isDirectParentOf(Identifier* identifier);

            std::string getName() { return this->name_; }
            IdentifierList* getDirectParents() { return &(this->directParents_); }
            IdentifierList* getAllParents() { return &(this->allParents_); }
            IdentifierList* getDirectChildren() { return &(this->directChildren_); }
            IdentifierList* getAllChildren() { return &(this->allChildren_); }

            static bool isCreatingHierarchy() { return (hierarchyCreatingCounter_s > 0); }

        private:
            Identifier();
            Identifier(const Identifier& identifier) {}
            virtual ~Identifier();
            void initialize(IdentifierList* parents);

            static void startCreatingHierarchy() { hierarchyCreatingCounter_s++; std::cout << "*** Increased Hierarchy-Creating-Counter to " << hierarchyCreatingCounter_s << "\n"; }
            static void stopCreatingHierarchy() { hierarchyCreatingCounter_s--; std::cout << "*** Decreased Hierarchy-Creating-Counter to " << hierarchyCreatingCounter_s << "\n"; }

            IdentifierList directParents_;
            IdentifierList allParents_;
            IdentifierList directChildren_;
            IdentifierList allChildren_;

            ObjectList objects_;
            std::string name_;

            bool bIsAbstractClass_;
            bool bCreatedOneObject_;

            static int hierarchyCreatingCounter_s;
    };


    // ##### ClassIdentifier #####
    template <class T>
    class ClassIdentifier : public Identifier
    {
        public:
            static ClassIdentifier<T>* registerClass(IdentifierList* parents, std::string name, bool bRootClass, bool bIsAbstractClass);
            static ClassIdentifier<T>* getIdentifier();
            BaseObject* fabricate();
            T* fabricateClass();

        private:
            ClassIdentifier();
            ClassIdentifier(const ClassIdentifier<T>& identifier) {}
            ~ClassIdentifier();

            static ClassIdentifier<T>* pointer_s;

    };

    template <class T>
    ClassIdentifier<T>* ClassIdentifier<T>::pointer_s = NULL;

    template <class T>
    ClassIdentifier<T>::ClassIdentifier()
    {
    }

    template <class T>
    ClassIdentifier<T>::~ClassIdentifier()
    {
        this->pointer_s = NULL;
    }

    template <class T>
    BaseObject* ClassIdentifier<T>::fabricate()
    {
        return dynamic_cast<BaseObject*>(this->fabricateClass());
    }

    template <class T>
    T* ClassIdentifier<T>::fabricateClass()
    {
        if (!this->bIsAbstractClass_)
        {
            return new T;
        }
        else
        {
            std::cout << "Error: Couldn't create a new Object - Class " << this->name_ << " is abstract!\n";
            std::cout << "Aborting...\n";
            abort();
        }
    }

    template <class T>
    ClassIdentifier<T>* ClassIdentifier<T>::registerClass(IdentifierList* parents, std::string name, bool bRootClass, bool bIsAbstractClass)
    {
        std::cout << "*** Register Class in " << name << "-Singleton.\n";
        if (!pointer_s)
        {
            std::cout << "*** Register Class in " << name << "-Singleton -> Create Singleton.\n";
            if (parents || bRootClass)
            {
                pointer_s = new ClassIdentifier();
                pointer_s->name_ = name;
                pointer_s->bIsAbstractClass_ = bIsAbstractClass;

                ClassFactory::add(name, pointer_s);

                if (!bRootClass)
                    pointer_s->initialize(parents);
                else
                    pointer_s->initialize(NULL);
            }
            else
            {
                pointer_s = getIdentifier();
            }
        }

        return pointer_s;
    }

    template <class T>
    ClassIdentifier<T>* ClassIdentifier<T>::getIdentifier()
    {
//        std::cout << "*** Get Identifier.\n";
        if (!pointer_s)
        {
            std::cout << "*** Get Identifier -> Create Class\n";
            Identifier::startCreatingHierarchy();
            T* temp = new T();
            delete temp;
            Identifier::stopCreatingHierarchy();
        }

        return pointer_s;
    }

    // ##### BaseIdentifier #####
    template <class B>
    class BaseIdentifier
    {
        public:
            BaseIdentifier();

            BaseIdentifier<B>& operator=(Identifier* identifier)
            {
                if (!identifier->isA(ClassIdentifier<B>::getIdentifier()))
                {
                    std::cout << "Error: Class " << identifier->getName() << " is not a " << ClassIdentifier<B>::getIdentifier()->getName() << "!\n";
                    std::cout << "Error: BaseIdentifier<" << ClassIdentifier<B>::getIdentifier()->getName() << "> = Class(" << identifier->getName() << ") is forbidden.\n";
                    std::cout << "Aborting...\n";
                    abort();
                }
                this->identifier_ = identifier;
                return *this;
            }

            Identifier* operator*()
            {
                return this->identifier_;
            }

            Identifier* operator->() const
            {
                return this->identifier_;
            }

            B* fabricate()
            {
                BaseObject* newObject = this->identifier_->fabricate();
                if (newObject)
                {
                    return dynamic_cast<B*>(newObject);
                }
                else
                {
                    if (this->identifier_)
                    {
                        std::cout << "Error: Class " << this->identifier_->getName() << " is not a " << ClassIdentifier<B>::getIdentifier()->getName() << "!\n";
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

            inline Identifier* getIdentifier()
                { return this->identifier_; }
            inline bool isA(Identifier* identifier)
                { return this->identifier_->isA(identifier); }
            inline bool isDirectlyA(Identifier* identifier)
                { return this->identifier_->isDirectlyA(identifier); }
            inline bool isChildOf(Identifier* identifier)
                { return this->identifier_->isChildOf(identifier); }
            inline bool isDirectChildOf(Identifier* identifier)
                { return this->identifier_->isDirectChildOf(identifier); }
            inline bool isParentOf(Identifier* identifier)
                { return this->identifier_->isParentOf(identifier); }
            inline bool isDirectParentOf(Identifier* identifier)
                { return this->identifier_->isDirectParentOf(identifier); }

        private:
            Identifier* identifier_;
    };

    template <class B>
    BaseIdentifier<B>::BaseIdentifier()
    {
        this->identifier_ = ClassIdentifier<B>::getIdentifier();
    }
}

#endif
