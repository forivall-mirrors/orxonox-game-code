#include "BaseObject.h"

namespace orxonox
{
    CreateFactory(BaseObject);

    BaseObject::BaseObject()
    {
        registerRootObject(BaseObject);
    }

    BaseObject::~BaseObject()
    {
        unregisterObject();
    }
}
