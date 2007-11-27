#include "MetaObjectList.h"

namespace orxonox
{
    MetaObjectList::MetaObjectList()
    {
        this->first_ = 0;
    }

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
