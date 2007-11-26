#include "OrxonoxClass.h"

namespace orxonox
{
    OrxonoxClass::OrxonoxClass()
    {
        this->identifier_ = 0;
        this->parents_ = 0;

        this->metaList_ = new MetaObjectList;
    }

    OrxonoxClass::~OrxonoxClass()
    {
        if (this->parents_)
            delete this->parents_;

        delete this->metaList_;
    }
}
