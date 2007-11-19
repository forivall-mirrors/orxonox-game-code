#ifndef _Identifier_H__
#define _Identifier_H__

#include "ClassHierarchy.h"
#include "IdentifierList.h"
#include "ObjectList.h"
#include "OrxonoxClass.h"
#include "Factory.h"

namespace orxonox
{
    // ##### Identifier #####
    class Identifier
    {
        template <class T>
        friend class ClassIdentifier;

        template <class T>
        friend class BaseIdentifier;

        public:
            void addObject(OrxonoxClass* object);
            void removeObject(OrxonoxClass* object);

            virtual OrxonoxClass* fabricate() {};

            bool isA(Identifier* identifier);
            bool isDirectlyA(Identifier* identifier);
            bool isChildOf(Identifier* identifier);
            bool isDirectChildOf(Identifier* identifier);
            bool isParentOf(Identifier* identifier);
            bool isDirectParentOf(Identifier* identifier);

            std::string getName() { return this->name_; }
            IdentifierList* getDirectParents() { return this->directParents_; }
            IdentifierList* getAllParents() { return this->allParents_; }
            IdentifierList* getDirectChildren() { return this->directChildren_; }
            IdentifierList* getAllChildren() { return this->allChildren_; }

        private:
            Identifier();
            Identifier(const Identifier& identifier) {}
            virtual ~Identifier();
            void initialize(IdentifierList* parents);

            IdentifierList* directParents_;
            IdentifierList* allParents_;
            IdentifierList* directChildren_;
            IdentifierList* allChildren_;

            ObjectList* objects_;
            std::string name_;

            bool bIsAbstractClass_;
            bool bCreatedOneObject_;
    };


    // ##### ClassIdentifier #####
    template <class T>
    class ClassIdentifier : public Identifier
    {
        public:
            static ClassIdentifier<T>* registerClass(IdentifierList* parents, std::string name, bool bRootClass, bool bIsAbstractClass);
            static ClassIdentifier<T>* getIdentifier();
            OrxonoxClass* fabricate();
            T* fabricateClass();

        private:
            ClassIdentifier();
            ClassIdentifier(const ClassIdentifier<T>& identifier) {}
            ~ClassIdentifier();

            static ClassIdentifier<T>* pointer_;

    };

    template <class T>
    ClassIdentifier<T>* ClassIdentifier<T>::pointer_ = NULL;

    template <class T>
    ClassIdentifier<T>::ClassIdentifier()
    {
    }

    template <class T>
    ClassIdentifier<T>::~ClassIdentifier()
    {
        this->pointer_ = NULL;
    }

    template <class T>
    OrxonoxClass* ClassIdentifier<T>::fabricate()
    {
        return this->fabricateClass();
    }

    template <class T>
    T* ClassIdentifier<T>::fabricateClass()
    {
        return new T;
    }

    template <class T>
    ClassIdentifier<T>* ClassIdentifier<T>::registerClass(IdentifierList* parents, std::string name, bool bRootClass, bool bIsAbstractClass)
    {
        std::cout << "*** Register Class in " << name << "-Singleton.\n";
        if (!pointer_)
        {
            std::cout << "*** Register Class in " << name << "-Singleton -> Create Singleton.\n";
            if (parents || bRootClass)
            {
                pointer_ = new ClassIdentifier();
                pointer_->name_ = name;
                pointer_->bIsAbstractClass_ = bIsAbstractClass;

                ClassFactory::add(name, pointer_);

                pointer_->initialize(parents);
            }
            else
            {
                pointer_ = getIdentifier();
            }
        }

        return pointer_;
    }

    template <class T>
    ClassIdentifier<T>* ClassIdentifier<T>::getIdentifier()
    {
//        std::cout << "*** Get Identifier.\n";
        if (!pointer_)
        {
            std::cout << "*** Get Identifier -> Create Class\n";
            ClassHierarchy::getSingleton()->startCreatingHierarchy();
            T* temp = new T();
            ClassHierarchy::getSingleton()->stopCreatingHierarchy();
            delete temp;
        }

        return pointer_;
    }

    // ##### BaseIdentifier #####
    template <class B>
    class BaseIdentifier
    {
        public:
            BaseIdentifier();

            BaseIdentifier<B>& operator= (Identifier* identifier)
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

            Identifier* operator* ()
            {
                return this->identifier_;
            }

            Identifier* operator-> () const
            {
                return this->identifier_;
            }

            B* fabricate()
            {
                OrxonoxClass* newObject = this->identifier_->fabricate();
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
