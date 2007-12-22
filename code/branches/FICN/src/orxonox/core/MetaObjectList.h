/*!
    @file MetaObjectList.h
    @brief Definition of the MetaObjectList class.

    The MetaObjectList is a single-linked list, containing all list-elements and their
    lists wherein the object, owning the MetaObjectList, is registered.
    This allows much faster deletion of objects because no iteration is needed.
*/

#ifndef _MetaObjectList_H__
#define _MetaObjectList_H__

#include "ObjectList.h"
#include "Identifier.h"
#include "Debug.h"

namespace orxonox
{
    //! Base-class of MetaObjectListElement, because those is a template
    class BaseMetaObjectListElement
    {
        public:
            /** @brief Default destructor */
            virtual ~BaseMetaObjectListElement() {};

            BaseMetaObjectListElement* next_;       //!< The next Element in the list
    };

    // ###############################
    // ###  MetaObjectListElement  ###
    // ###############################
    //! The list-element of the MetaObjectList
    template <class T>
    class MetaObjectListElement : public BaseMetaObjectListElement
    {
        public:
            MetaObjectListElement(ObjectList<T>* list, ObjectListElement<T>* element);
            ~MetaObjectListElement();

            ObjectListElement<T>* element_;         //!< The list element, containing the object
            ObjectList<T>* list_;                   //!< The list, containing the element
    };

    /**
        @brief Constructor: Creates the list-element with given list and element.
    */
    template <class T>
    MetaObjectListElement<T>::MetaObjectListElement(ObjectList<T>* list, ObjectListElement<T>* element)
    {
        this->element_ = element;
        this->list_ = list;
        this->next_ = 0;
    }

    /**
        @brief Destructor: Removes the ObjectListElement from the ObjectList by linking next_ and prev_ of the ObjectListElement.
    */
    template <class T>
    MetaObjectListElement<T>::~MetaObjectListElement()
    {
        if (this->element_->next_)
            this->element_->next_->prev_ = this->element_->prev_;
        else
            this->list_->last_ = this->element_->prev_; // If there is no next_, we deleted the last object and have to update the last_ pointer of the list

        if (this->element_->prev_)
            this->element_->prev_->next_ = this->element_->next_;
        else
            this->list_->first_ = this->element_->next_; // If there is no prev_, we deleted the first object and have to update the first_ pointer of the list


        COUT(4) << "*** Removing Object from " << ClassIdentifier<T>::getIdentifier()->getName() << "-list.\n";
        delete this->element_;
    }


    // ###############################
    // ###       ObjectList        ###
    // ###############################
    //!  The MetaObjectList contains ObjectListElements and their ObjectLists.
    /**
        The MetaObjectList is a single-linked list, containing all list-elements and their
        lists wherein the object that owns the MetaObjectList is registered.
        This allows much faster deletion of objects because no iteration is needed.
    */
    class MetaObjectList
    {
        public:
            MetaObjectList();
            ~MetaObjectList();
            template <class T>
            void add(ObjectList<T>* list, ObjectListElement<T>* element);

            BaseMetaObjectListElement* first_;      //!< The first element in the list
    };

    /**
        @brief Adds an ObjectList and an element of that list to the MetaObjectList.
        @param list The ObjectList wherein the element is
        @param element The element wherein the object is
    */
    template <class T>
    void MetaObjectList::add(ObjectList<T>* list, ObjectListElement<T>* element)
    {
        BaseMetaObjectListElement* temp = this->first_;
        this->first_ = new MetaObjectListElement<T>(list, element);
        this->first_->next_ = temp;
    }
}

#endif /* _MetaObjectList_H__ */
