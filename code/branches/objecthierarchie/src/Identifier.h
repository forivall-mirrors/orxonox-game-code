#ifndef _Identifier_H__
#define _Identifier_H__

#include <iostream>

#include "IdentifierList.h"
#include "ObjectList.h"
#include "Factory.h"

#define HIERARCHY_VERBOSE true


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
        friend class SubclassIdentifier;

        template <class T>
        friend class ClassFactory;

        public:
            virtual void removeObject(OrxonoxClass* object) const = 0;
            virtual void removeObjectIntern(OrxonoxClass* object, bool bIterateForwards) const = 0;

            inline void addFactory(BaseFactory* factory) { this->factory_ = factory; }
            BaseObject* fabricate();

            bool isA(const Identifier* identifier) const;
            bool isDirectlyA(const Identifier* identifier) const;
            bool isChildOf(const Identifier* identifier) const;
            bool isParentOf(const Identifier* identifier) const;

            inline const std::string& getName() const { return this->name_; }
            inline const IdentifierList& getParents() const { return this->parents_; }
            inline IdentifierList& getChildren() const { return *this->children_; }

            inline static bool isCreatingHierarchy() { return (hierarchyCreatingCounter_s > 0); }

        private:
            Identifier();
            Identifier(const Identifier& identifier) {}
            virtual ~Identifier();
            void initialize(const IdentifierList* parents);

            inline static void startCreatingHierarchy()
            {
                hierarchyCreatingCounter_s++;
#if HIERARCHY_VERBOSE
                std::cout << "*** Increased Hierarchy-Creating-Counter to " << hierarchyCreatingCounter_s << "\n";
#endif
            }

            inline static void stopCreatingHierarchy()
            {
                hierarchyCreatingCounter_s--;
#if HIERARCHY_VERBOSE
                std::cout << "*** Decreased Hierarchy-Creating-Counter to " << hierarchyCreatingCounter_s << "\n";
#endif
            }

            IdentifierList parents_;
            IdentifierList* children_;

            std::string name_;

            BaseFactory* factory_;
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
            static ClassIdentifier<T>* registerClass(const IdentifierList* parents, const std::string& name, bool bRootClass);
            static ClassIdentifier<T>* getIdentifier();
            static void addObject(T* object);
            void removeObject(OrxonoxClass* object) const;
            void removeObjectIntern(OrxonoxClass* object, bool bIterateForwards) const;

        private:
            ClassIdentifier();
            ClassIdentifier(const ClassIdentifier<T>& identifier) {}
            ~ClassIdentifier();

            static ClassIdentifier<T>* pointer_s;
            ObjectList<T>* objects_;
    };

    template <class T>
    ClassIdentifier<T>* ClassIdentifier<T>::pointer_s = NULL;

    template <class T>
    ClassIdentifier<T>::ClassIdentifier()
    {
        this->objects_ = new ObjectList<T>;
    }

    template <class T>
    ClassIdentifier<T>::~ClassIdentifier()
    {
        delete this->objects_;
        this->pointer_s = NULL;
    }

    template <class T>
    ClassIdentifier<T>* ClassIdentifier<T>::registerClass(const IdentifierList* parents, const std::string& name, bool bRootClass)
    {
#if HIERARCHY_VERBOSE
        std::cout << "*** Register Class in " << name << "-Singleton.\n";
#endif
        if (!pointer_s)
        {
#if HIERARCHY_VERBOSE
            std::cout << "*** Register Class in " << name << "-Singleton -> Create Singleton.\n";
#endif
            pointer_s = new ClassIdentifier();
        }

        if (!pointer_s->bCreatedOneObject_)
        {
#if HIERARCHY_VERBOSE
            std::cout << "*** Register Class in " << name << "-Singleton -> Initialize Singleton.\n";
#endif
            pointer_s->name_ = name;
            Factory::add(name, pointer_s);

            if (bRootClass)
                pointer_s->initialize(NULL);
            else
                pointer_s->initialize(parents);
        }

        return pointer_s;
    }

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

    template <class T>
    void ClassIdentifier<T>::addObject(T* object)
    {
#if HIERARCHY_VERBOSE
        std::cout << "*** Added object to " << ClassIdentifier<T>::getIdentifier()->getName() << "-list.\n";
#endif
        ClassIdentifier<T>::getIdentifier()->objects_->add(object);
    }

    template <class T>
    void ClassIdentifier<T>::removeObject(OrxonoxClass* object) const
    {
        bool bIterateForwards = !Identifier::isCreatingHierarchy();

        this->removeObjectIntern(object, bIterateForwards);

        IdentifierListElement* temp = this->parents_.first_;
        while (temp)
        {
            temp->identifier_->removeObjectIntern(object, bIterateForwards);
            temp = temp->next_;
        }
    }

    template <class T>
    void ClassIdentifier<T>::removeObjectIntern(OrxonoxClass* object, bool bIterateForwards) const
    {
#if HIERARCHY_VERBOSE
        if (bIterateForwards)
            std::cout << "*** Removed object from " << this->name_ << "-list, iterating forwards.\n";
        else
            std::cout << "*** Removed object from " << this->name_ << "-list, iterating backwards.\n";
#endif

        this->objects_->remove(object, bIterateForwards);
    }


    // ###############################
    // ###   SubclassIdentifier    ###
    // ###############################
    template <class B>
    class SubclassIdentifier
    {
        public:
            SubclassIdentifier();

            SubclassIdentifier<B>& operator=(Identifier* identifier)
            {
                if (!identifier->isA(ClassIdentifier<B>::getIdentifier()))
                {
                    std::cout << "Error: Class " << identifier->getName() << " is not a " << ClassIdentifier<B>::getIdentifier()->getName() << "!\n";
                    std::cout << "Error: SubclassIdentifier<" << ClassIdentifier<B>::getIdentifier()->getName() << "> = Class(" << identifier->getName() << ") is forbidden.\n";
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

            inline const Identifier* getIdentifier() const
                { return this->identifier_; }
            inline bool isA(const Identifier* identifier) const
                { return this->identifier_->isA(identifier); }
            inline bool isDirectlyA(const Identifier* identifier) const
                { return this->identifier_->isDirectlyA(identifier); }
            inline bool isChildOf(const Identifier* identifier) const
                { return this->identifier_->isChildOf(identifier); }
            inline bool isParentOf(const Identifier* identifier) const
                { return this->identifier_->isParentOf(identifier); }

        private:
            Identifier* identifier_;
    };

    template <class B>
    SubclassIdentifier<B>::SubclassIdentifier()
    {
        this->identifier_ = ClassIdentifier<B>::getIdentifier();
    }
}

#endif
