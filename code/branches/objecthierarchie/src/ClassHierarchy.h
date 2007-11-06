#ifndef _ClassHierarchy_H__
#define _ClassHierarchy_H__

#include <string>
#include <iostream>

// DONE AND TESTED:
// - build class hierarchy
// - isA, isChildOf, ...
// - insert into class-lists
// - ClassIdentifier

// IN WORK:
// - BaseIdentifier
// - Factory

// TO DO:
// - iterate through lists
// - searchtree for classname-strings


namespace orxonox
{
    // ##### ClassHierarchy #####
    template <class T>
    class ClassIdentifier;

    class ClassHierarchy
    {
        template <class T>
        friend class ClassIdentifier;

        public:
            static ClassHierarchy* getSingleton();
            bool isCreatingHierarchy() { return (this->hierarchyCreatingCounter_ > 0); }

        private:
            ClassHierarchy();
            ~ClassHierarchy();
            void startCreatingHierarchy() { this->hierarchyCreatingCounter_++; std::cout << "*** Increased Hierarchy-Creating-Counter to " << this->hierarchyCreatingCounter_ << "\n"; }
            void stopCreatingHierarchy() { this->hierarchyCreatingCounter_--; std::cout << "*** Decreased Hierarchy-Creating-Counter to " << this->hierarchyCreatingCounter_ << "\n"; }

            static ClassHierarchy* pointer_;
            int hierarchyCreatingCounter_;
    };

    // ##### Identifier #####
    class IdentifierList;
    class ObjectList;
    class OrxonoxClass;

    class Identifier
    {
        template <class T>
        friend class ClassIdentifier;

        template <class T>
        friend class BaseIdentifier;

        public:
            void addObject(OrxonoxClass* object);
            void removeObject(OrxonoxClass* object);

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

            bool bCreatedOneObject_;
    };


    // ##### ClassIdentifier #####
    template <class T>
    class ClassIdentifier : public Identifier
    {
        public:
            static ClassIdentifier<T>* registerClass(IdentifierList* parents, std::string name, bool bRootClass);
            static ClassIdentifier<T>* getIdentifier();
            static T* create();

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

    template <class T>
    T* ClassIdentifier<T>::create()
    {
        return new T();
    }


    // ##### BaseIdentifier #####
    template <class B>
    class BaseIdentifier// : public Identifier
    {
        public:
            BaseIdentifier();

            template <class T>
            BaseIdentifier<B>& operator= (ClassIdentifier<T>* identifier)
            {
                std::cout << "####### Class(" << identifier->getName() << ")->isA( Class(" << ClassIdentifier<B>::getIdentifier()->getName() << ") ) = " << identifier->isA( ClassIdentifier<B>::getIdentifier() ) << "\n";
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

            operator Identifier()
            {
                return this->identifier_;
            }

        private:
            Identifier* identifier_;
    };

    template <class B>
    BaseIdentifier<B>::BaseIdentifier()
    {
        this->identifier_ = ClassIdentifier<B>::getIdentifier();
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
            std::string toString();

            IdentifierListElement* first_;
    };

    class IdentifierListElement
    {
        public:
            IdentifierListElement(Identifier* identifier);
            ~IdentifierListElement();

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
            ~ObjectListElement();

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
