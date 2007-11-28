#include "BaseObject.h"

namespace orxonox
{
    BaseObject::BaseObject()
    {
        registerRootObject(BaseObject);
    }

    BaseObject::~BaseObject()
    {
        unregisterObject();
    }
}
