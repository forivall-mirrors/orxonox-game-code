/*!
    @file BaseObject.cc
    @brief Implementation of the BaseObject class.
*/

#include "BaseObject.h"

namespace orxonox
{
    CreateFactory(BaseObject);

    /**
        @brief Constructor: Registers the object in the BaseObject-list.
    */
    BaseObject::BaseObject()
    {
        RegisterRootObject(BaseObject);
    }

    /**
        @brief Destructor
    */
    BaseObject::~BaseObject()
    {
    }
}
