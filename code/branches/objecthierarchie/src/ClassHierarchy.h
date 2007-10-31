#ifndef _ClassHierarchy_H__
#define _ClassHierarchy_H__

#include <string>

// DONE:
// - klassenhierarchie aufbauen
// - klassen-identifier

// TODO:
// - durch listen iterieren
// - isA usw vergleiche
// - new überladen + objekt in liste eintragen
// - searchtree für classname-strings

//namespace orxonox
//{
    class ClassName;
    class ClassList;
    class ObjectList;
    class BaseObject;

    template <class T>
    class ClassNameSingleton
    {
        public:
            static ClassName* getClassName(BaseObject* object, bool bIsRootClass);
            static ClassName* getNewClassName();
            BaseObject* create();

        private:
            ClassNameSingleton();
            ~ClassNameSingleton();
            static ClassNameSingleton *pointer;
            static ClassName *className;
    };

    class ClassName
    {
        public:
            ClassName(const std::string& name, ClassNameSingleton<class T>* factory);
            ~ClassName();

            std::string name;
            ClassName *parentClass;
            ClassList *childClasses;
            ObjectList *objects;
            ClassNameSingleton<class T> *factory;
    };

    class ClassListItem
    {
        public:
            ClassListItem(ClassName* className);
            ~ClassListItem();

            ClassListItem *next;
            ClassName *className;
    };

    class ClassList
    {
        public:
            ClassList();
            ~ClassList();
            void add(ClassName* className);

            ClassListItem *first;
    };

    class ObjectListItem
    {
        public:
            ObjectListItem(BaseObject* object);
            ~ObjectListItem();

            ObjectListItem *next;
            BaseObject *object;
    };

    class ObjectList
    {
        public:
            ObjectList();
            ~ObjectList();
            void add(BaseObject* object);
            void remove(BaseObject* object);

            ObjectListItem *first;
    };

    class ClassNameTree
    {
        public:
            static ClassNameTree* getSingleton();
            BaseObject* create(ClassName* className);
            BaseObject* create(std::string& name);
            ClassName* getClassName(std::string& name);
            ClassName* getClassName(std::string& name, ClassName* root);
            ClassName* getRootClass() { return this->rootClass; }
            void setRootClass(ClassName* className) { this->rootClass = className; }

        private:
            ClassNameTree();
            ~ClassNameTree();
            static ClassNameTree *pointer;
            ClassName* rootClass;
    };


    #define className(ClassName) \
        ClassNameSingleton<ClassName>::getNewClassName()

    #define registerObject(ClassName, bIsRootClass) \
        this->className = ClassNameSingleton<ClassName>::getClassName(this, bIsRootClass)

    #define unregisterObject() \
        this->className->objects->remove(this)

    #define factory(ClassName) \
        ClassNameTree::getSingleton()->create(ClassName)
//}

#endif
