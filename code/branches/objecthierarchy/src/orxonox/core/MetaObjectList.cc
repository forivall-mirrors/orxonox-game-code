/*!
    @file MetaObjectList.cc
    @brief Implementation of the MetaObjectList class.
*/

#include "MetaObjectList.h"

namespace orxonox
{
    /**
        @brief Constructor: Sets first_ to zero.
    */
    MetaObjectList::MetaObjectList()
    {
        this->first_ = 0;
    }

    /**
        @brief Destructor: Removes all elements from the list, causing them to remove the stored ObjectListElement from the ObjectList.
    */
    MetaObjectList::~MetaObjectList()
    {
        BaseMetaObjectListElement* temp;
        while (this->first_)
        {
            temp = this->first_->next_;
            delete this->first_;
            this->first_ = temp;
        }
    }
}
