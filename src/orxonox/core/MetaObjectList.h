#ifndef _MetaObjectList_H__
#define _MetaObjectList_H__

#include "ObjectList.h"
#include "Identifier.h"

namespace orxonox
{
    class BaseMetaObjectListElement
    {
        public:
            virtual ~BaseMetaObjectListElement() {};

            BaseMetaObjectListElement* next_;
    };

    // ###############################
    // ###  MetaObjectListElement  ###
    // ###############################
    template <class T>
    class MetaObjectListElement : public BaseMetaObjectListElement
    {
        public:
            MetaObjectListElement(ObjectList<T>* list, ObjectListElement<T>* element);
            ~MetaObjectListElement();

            ObjectListElement<T>* element_;
            ObjectList<T>* list_;
    };

    template <class T>
    MetaObjectListElement<T>::MetaObjectListElement(ObjectList<T>* list, ObjectListElement<T>* element)
    {
        this->element_ = element;
        this->list_ = list;
        this->next_ = 0;
    }

    template <class T>
    MetaObjectListElement<T>::~MetaObjectListElement()
    {
        if (this->element_->next_)
            this->element_->next_->prev_ = this->element_->prev_;
        else
            this->list_->last_ = this->element_->prev_;

        if (this->element_->prev_)
            this->element_->prev_->next_ = this->element_->next_;
        else
            this->list_->first_ = this->element_->next_;


#if HIERARCHY_VERBOSE
        std::cout << "*** Removing Object from " << ClassIdentifier<T>::getIdentifier()->getName() << "-list.\n";
#endif
        delete this->element_;
    }


    // ###############################
    // ###       ObjectList        ###
    // ###############################
    class MetaObjectList
    {
        public:
            MetaObjectList();
            ~MetaObjectList();
            template <class T>
            void add(ObjectList<T>* list, ObjectListElement<T>* element);

            BaseMetaObjectListElement* first_;
    };

    template <class T>
    void MetaObjectList::add(ObjectList<T>* list, ObjectListElement<T>* element)
    {
        BaseMetaObjectListElement* temp = this->first_;
        this->first_ = new MetaObjectListElement<T>(list, element);
        this->first_->next_ = temp;
    }
}

#endif
