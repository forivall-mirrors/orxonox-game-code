#ifndef _ClassHierarchy_H__
#define _ClassHierarchy_H__

#include <string>
#include <iostream>

// DONE:
// - klassenhierarchie aufbauen
// - in listen einfügen
// - factory
// - klassen-identifier
// - isA u.ä. vergleiche

// TODO:
// - durch listen iterieren
// - searchtree für classname-strings


namespace orxonox
{
    // ##### ClassHierarchy #####
    class ClassHierarchy
    {
        public:
            static ClassHierarchy* getSingleton();
            bool isCreatingHierarchy() { return this->bCreatingHierarchy_; }
            void createHierarchy(bool bCreatingHierarchy) { this->bCreatingHierarchy_ = bCreatingHierarchy; std::cout << "*** Switched Hierarchy-Creating-Mode to" << bCreatingHierarchy << "\n"; }

        private:
            ClassHierarchy();

            static ClassHierarchy* pointer_;
            bool bCreatingHierarchy_;
    };

    // ##### Identifier #####
    class IdentifierList;
    class ObjectList;
    class OrxonoxClass;
    template <class T>
    class ClassIdentifier;

    class Identifier
    {
        template <class T>
        friend class ClassIdentifier;

        public:
            void addObject(OrxonoxClass* object);
            void removeObject(OrxonoxClass* object);

            bool isA(Identifier* identifier);
            bool isDirectA(Identifier* identifier);
            bool isChildOf(Identifier* identifier);
            bool isDirectChildOf(Identifier* identifier);
            bool isParentOf(Identifier* identifier);
            bool isDirectParentOf(Identifier* identifier);

        private:
            Identifier();
            void initialize(IdentifierList* parents);

            IdentifierList* directParents_;
            IdentifierList* allParents_;
            IdentifierList* directChildren_;
            IdentifierList* allChildren_;

            ObjectList* objects_;
            std::string name_;

            bool bCreatedOneObject_;
    };

    template <class T>
    class ClassIdentifier : public Identifier
    {
        public:
            static ClassIdentifier<T>* registerClass(IdentifierList* parents, std::string name, bool bRootClass);
            static ClassIdentifier<T>* getIdentifier();
            static T* create();

        private:
            ClassIdentifier();

            static ClassIdentifier<T>* pointer_;

    };

    template <class T>
    ClassIdentifier<T>* ClassIdentifier<T>::pointer_ = NULL;

    template <class T>
    ClassIdentifier<T>::ClassIdentifier()
    {
    }

    template <class T>
    ClassIdentifier<T>* ClassIdentifier<T>::registerClass(IdentifierList* parents, std::string name, bool bRootClass)
    {
        std::cout << "*** Register Class in " << name << "-Singleton.\n";
        if (!pointer_)
        {
            std::cout << "*** Register Class in " << name << "-Singleton -> Create Singleton.\n";
            if (parents || bRootClass)
            {
                pointer_ = new ClassIdentifier();
                pointer_->name_ = name;
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
        std::cout << "*** Get Identifier.\n";
        if (!pointer_)
        {
            std::cout << "*** Get Identifier -> Create Class\n";
            ClassHierarchy::getSingleton()->createHierarchy(true);
            T* temp = new T();
            ClassHierarchy::getSingleton()->createHierarchy(false);
            delete temp;
        }

        return pointer_;
    }

    template <class T>
    T* ClassIdentifier<T>::create()
    {
        return new T();
    }

    // ##### Identifier List #####
    class IdentifierListElement;

    class IdentifierList
    {
        public:
            IdentifierList();
            ~IdentifierList();
            void add(Identifier* identifier);
            void remove(Identifier* identifier);
            bool isInList(Identifier* identifier);

            IdentifierListElement* first_;
    };

    class IdentifierListElement
    {
        public:
            IdentifierListElement(Identifier* identifier);

            Identifier* identifier_;
            IdentifierListElement* next_;
            bool bDirect_;
    };


    // ##### Object List #####
    class ObjectListElement;

    class ObjectList
    {
        public:
            ObjectList();
            ~ObjectList();
            void add(OrxonoxClass* object);
            void remove(OrxonoxClass* object);

            ObjectListElement* first_;
    };

    class ObjectListElement
    {
        public:
            ObjectListElement(OrxonoxClass* object);

            OrxonoxClass* object_;
            ObjectListElement* next_;
    };

    // ##### Macros #####
    #define registerRootObject(ClassName) \
        std::cout << "*** Register Root-Object: " << #ClassName << "\n"; \
        if (ClassHierarchy::getSingleton()->isCreatingHierarchy() && !this->getParents()) \
            this->setParents(new IdentifierList()); \
        if (this->getIdentifier()) \
            this->getIdentifier()->removeObject(this); \
        this->setIdentifier(ClassIdentifier<ClassName>::registerClass(this->getParents(), #ClassName, true)); \
        if (ClassHierarchy::getSingleton()->isCreatingHierarchy() && this->getParents()) \
            this->getParents()->add(this->getIdentifier()); \
        this->getIdentifier()->addObject(this)

    #define registerObject(ClassName) \
        std::cout << "*** Register Object: " << #ClassName << "\n"; \
        this->getIdentifier()->removeObject(this); \
        this->setIdentifier(ClassIdentifier<ClassName>::registerClass(this->getParents(), #ClassName, false)); \
        if (ClassHierarchy::getSingleton()->isCreatingHierarchy() && this->getParents()) \
            this->getParents()->add(this->getIdentifier()); \
        this->getIdentifier()->addObject(this)

    #define unregisterObject() \
        this->getIdentifier()->removeObject(this)

    #define Class(ClassName) \
        ClassIdentifier<ClassName>::getIdentifier()

    #define Factory(ClassName) \
        ClassIdentifier<ClassName>::create()
}

#endif
