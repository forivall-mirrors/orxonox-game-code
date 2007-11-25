#include "BaseObject.h"

namespace orxonox
{
    CreateFactory(BaseObject);

    BaseObject::BaseObject()
    {
        RegisterRootObject(BaseObject);
    }

    BaseObject::~BaseObject()
    {
        UnregisterObject();
    }
}
