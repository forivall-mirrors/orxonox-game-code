#include "ClassHierarchy.h"
#include "BaseObject.h"

//namespace orxonox
//{
    // ##### ClassName #####
    ClassName::ClassName(const std::string& name, ClassNameSingleton<class T>* factory)
    {
        this->parentClass = NULL;
        this->childClasses = new ClassList();
        this->objects = new ObjectList();
        this->name = name;
        this->factory = factory;
    }

    ClassName::~ClassName()
    {
        delete this->childClasses;
        delete this->objects;
    }


    // ##### ClassListItem #####
    ClassListItem::ClassListItem(ClassName* className)
    {
        this->next = NULL;
        this->className = className;
    }

    ClassListItem::~ClassListItem()
    {
        delete this->className;
        delete this->next;
    }


    // ##### ClassList #####
    ClassList::ClassList()
    {
        this->first = NULL;
    }

    ClassList::~ClassList()
    {
        delete this->first;
    }

    void ClassList::add(ClassName* className)
    {
        if (!this->first)
        {
            this->first = new ClassListItem(className);
            return;
        }

        ClassListItem* iterator = this->first;
        while (iterator != NULL)
        {
            if (iterator->next == NULL)
            {
                iterator->next = new ClassListItem(className);
                return;
            }

            iterator = iterator->next;
        }
    }

    // ##### ObjectListItem #####
    ObjectListItem::ObjectListItem(BaseObject* object)
    {
        this->next = NULL;
        this->object = object;
    }

    ObjectListItem::~ObjectListItem()
    {
        delete this->object;
        delete this->next;
    }


    // ##### ObjectList #####
    ObjectList::ObjectList()
    {
        this->first = NULL;
    }

    ObjectList::~ObjectList()
    {
        delete this->first;
    }

    void ObjectList::add(BaseObject* object)
    {
        if (!this->first)
        {
            this->first = new ObjectListItem(object);
            return;
        }

        ObjectListItem* iterator = this->first;
        while (iterator != NULL)
        {
            if (iterator->next == NULL)
            {
                iterator->next = new ObjectListItem(object);
                return;
            }

            iterator = iterator->next;
        }
    }

    void ObjectList::remove(BaseObject* object)
    {
        if (!this->first || !object)
            return;

        if (this->first->object == object)
        {
            ObjectListItem* temp = this->first->next;
            delete this->first;
            this->first = temp;

            return;
        }

        ObjectListItem* iterator = this->first;
        while (iterator->next != NULL)
        {
            if (iterator->next->object == object)
            {
                ObjectListItem* temp = iterator->next->next;
                delete iterator->next;
                iterator->next = temp;

                return;
            }

            iterator = iterator->next;
        }
    }

    // ##### ClassNameTree #####
    ClassNameTree* ClassNameTree::pointer = NULL;

    ClassNameTree::ClassNameTree()
    {
    }

    ClassNameTree::~ClassNameTree()
    {
        this->pointer = NULL;
    }

    ClassNameTree* ClassNameTree::getSingleton()
    {
        if (!pointer)
        {
            pointer = new ClassNameTree();
        }

        return pointer;
    }
/*
    BaseObject* ClassNameTree::create(ClassName* className)
    {
        return className->factory->create();
    }

    BaseObject* ClassNameTree::create(std::string& name)
    {
        return this->getClassName(name)->factory->create();
    }
*/
    ClassName* ClassNameTree::getClassName(std::string& name)
    {
        return getClassName(name, this->rootClass);
    }

    ClassName* ClassNameTree::getClassName(std::string& name, ClassName* root)
    {
        if (root->name == name)
            return root;

        ClassListItem* temp = root->childClasses->first;
        while (temp != NULL)
        {
            ClassName* temp2 = this->getClassName(name, temp->className);
            if (temp2)
                return temp2;

            temp = temp->next;
        }

        return NULL;
    }


    // ##### ClassNameSingleton #####
    #define getClassNameString(ClassName) \
        #ClassName

    template <class T>
    ClassNameSingleton<T>* ClassNameSingleton<T>::pointer = NULL;

    template <class T>
    ClassName* ClassNameSingleton<T>::className = NULL;

    template <class T>
    ClassName* ClassNameSingleton<T>::getClassName(BaseObject* object, bool bIsRootClass)
    {
        if (!pointer || !className)
        {
            pointer = new ClassNameSingleton<T>();
            className = new ClassName(getClassNameString(T), pointer);

            if (bIsRootClass)
            {
                className->parentClass = NULL;
                ClassNameTree::getSingleton()->setRootClass(className);
            }
            else
            {
                className->parentClass = object->className;
                object->className->childClasses->add(className);
            }
        }

        return className;
    }

    template <class T>
    ClassName* ClassNameSingleton<T>::getNewClassName()
    {
        if (!pointer || !className)
        {
            T* temp = new T();
            delete temp;
        }

        return className;
    }

    template <class T>
    BaseObject* ClassNameSingleton<T>::create()
    {
        return new T();
    }

    template <class T>
    ClassNameSingleton<T>::~ClassNameSingleton()
    {
        this->pointer = NULL;
        delete this->className;
        this->className = NULL;
    }
//}
