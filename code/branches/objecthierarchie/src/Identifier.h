#ifndef _Identifier_H__
#define _Identifier_H__

#include <iostream>

#include "IdentifierList.h"
#include "ObjectList.h"
#include "Factory.h"

#define HIERARCHY_VERBOSE false


namespace orxonox
{
    class BaseObject;

    // ###############################
    // ###       Identifier        ###
    // ###############################
    class Identifier
    {
        template <class T>
        friend class ClassIdentifier;

        template <class T>
        friend class BaseIdentifier;

        public:
            virtual void removeObject(OrxonoxClass* object) {};

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

            static void startCreatingHierarchy()
            {
                hierarchyCreatingCounter_s++;
#if HIERARCHY_VERBOSE
                std::cout << "*** Increased Hierarchy-Creating-Counter to " << hierarchyCreatingCounter_s << "\n";
#endif
            }

            static void stopCreatingHierarchy()
            {
                hierarchyCreatingCounter_s--;
#if HIERARCHY_VERBOSE
                std::cout << "*** Decreased Hierarchy-Creating-Counter to " << hierarchyCreatingCounter_s << "\n";
#endif
            }

            IdentifierList directParents_;
            IdentifierList allParents_;
            IdentifierList directChildren_;
            IdentifierList allChildren_;

            std::string name_;

            bool bIsAbstractClass_;
            bool bCreatedOneObject_;

            static int hierarchyCreatingCounter_s;
    };


    // ###############################
    // ###     ClassIdentifier     ###
    // ###############################
    template <class T>
    class ClassIdentifier : public Identifier
    {
        template <class U>
        friend class Iterator;

        public:
            static ClassIdentifier<T>* registerClass(IdentifierList* parents, std::string name, bool bRootClass, bool bIsAbstractClass);
            static ClassIdentifier<T>* getIdentifier();
            BaseObject* fabricate();
            T* fabricateClass();
            static void addObject(T* object);
            void removeObject(OrxonoxClass* object);

        private:
            ClassIdentifier();
            ClassIdentifier(const ClassIdentifier<T>& identifier) {}
            ~ClassIdentifier();

            static ClassIdentifier<T>* pointer_s;
            ObjectList<T> objects_s;
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
#if HIERARCHY_VERBOSE
        std::cout << "*** Register Class in " << name << "-Singleton.\n";
#endif
        if (!pointer_s)
        {
#if HIERARCHY_VERBOSE
            std::cout << "*** Register Class in " << name << "-Singleton -> Create Singleton.\n";
#endif
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
#if HIERARCHY_VERBOSE
//        std::cout << "*** Get Identifier.\n";
#endif
        if (!pointer_s)
        {
#if HIERARCHY_VERBOSE
            std::cout << "*** Get Identifier -> Create Class\n";
#endif
            Identifier::startCreatingHierarchy();
            T* temp = new T();
            delete temp;
            Identifier::stopCreatingHierarchy();
        }

        return pointer_s;
    }

    template <class T>
    void ClassIdentifier<T>::addObject(T* object)
    {
#if HIERARCHY_VERBOSE
        std::cout << "*** Added object to " << ClassIdentifier<T>::getIdentifier()->getName() << "-list.\n";
#endif
        ClassIdentifier<T>::getIdentifier()->objects_s.add(object);
    }

    template <class T>
    void ClassIdentifier<T>::removeObject(OrxonoxClass* object)
    {
        bool bIterateForwards = !Identifier::isCreatingHierarchy();

#if HIERARCHY_VERBOSE
        if (bIterateForwards)
            std::cout << "*** Removed object from " << this->name_ << "-list, iterating forwards.\n";
        else
            std::cout << "*** Removed object from " << this->name_ << "-list, iterating backwards.\n";
#endif

        this->objects_s.remove(object, bIterateForwards);

        IdentifierListElement* temp = this->directParents_.first_;
        while (temp)
        {
            temp->identifier_->removeObject(object);
            temp = temp->next_;
        }
    }


    // ###############################
    // ###     BaseIdentifier      ###
    // ###############################
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
