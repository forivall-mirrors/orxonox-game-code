#ifndef _ClassHierarchy_H__
#define _ClassHierarchy_H__

#include <string>

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
    // ##### Identifier #####
    class IdentifierList;
    class ObjectList;
    class BaseObject;

    class Identifier
    {
        template <class T>
        friend class ClassIdentifier;

        public:
//            static Identifier* registerClass(IdentifierList* parents);
            void addObject(BaseObject* object);
            void removeObject(BaseObject* object);

            bool isA(Identifier* identifier);
            bool isDirectA(Identifier* identifier);
            bool isChildOf(Identifier* identifier);
            bool isDirectChildOf(Identifier* identifier);
            bool isParentOf(Identifier* identifier);
            bool isDirectParentOf(Identifier* identifier);

        protected:
            Identifier();
            void initialize(IdentifierList* identifier);

            static Identifier* pointer_;

            IdentifierList* directParents_;
            IdentifierList* allParents_;
            IdentifierList* directChildren_;
            IdentifierList* allChildren_;

            ObjectList* objects_;
            std::string name_;

        private:
            bool bCreatedOneObject_;
    };

    template <class T>
    class ClassIdentifier : public Identifier
    {
//        friend class Identifier;

        public:
            static Identifier* registerClass(IdentifierList* parents);
            static Identifier* getIdentifier();
            static T* create();

        private:
            ClassIdentifier();
    };

    #define getStringFromClassName(ClassName) \
        #ClassName

    template <class T>
    ClassIdentifier<T>::ClassIdentifier()
    {
    }

    template <class T>
    Identifier* ClassIdentifier<T>::registerClass(IdentifierList* parents)
    {
        if (!pointer_)
        {
            pointer_ = new ClassIdentifier();
            pointer_->name_ = getStringFromClassName(T);
            pointer_->initialize(parents);
        }

        return pointer_;
    }

    template <class T>
    Identifier* ClassIdentifier<T>::getIdentifier()
    {
        if (!pointer_)
        {
            T* temp = new T();
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
            void add(BaseObject* object);
            void remove(BaseObject* object);

            ObjectListElement* first_;
    };

    class ObjectListElement
    {
        public:
            ObjectListElement(BaseObject* object);

            BaseObject* object_;
            ObjectListElement* next_;
    };


    // ##### Macros #####
    #define registerRootObject(ClassName) \
        this->parents_ = new IdentifierList(); \
        this->identifier_ = ClassIdentifier<ClassName>::registerClass(this->parents_); \
        this->parents_->add(this->identifier_); \
        this->identifier_->addObject(this)

    #define registerObject(ClassName) \
        this->identifier_->removeObject(this); \
        this->identifier_ = ClassIdentifier<ClassName>::registerClass(this->parents_); \
        this->parents_->add(this->identifier_); \
        this->identifier_->addObject(this)

    #define unregisterObject() \
        delete this->parents_; \
        this->identifier_->removeObject(this)

    #define Class(ClassName) \
        ClassIdentifier<ClassName>::getIdentifier()

    #define Factory(ClassName) \
        ClassIdentifier<ClassName>::create()
}

#endif
