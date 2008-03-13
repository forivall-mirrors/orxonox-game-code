#include "core/CoreIncludes.h"
#include "Tickable.h"

namespace orxonox
{
    /**
        @brief Constructor: Registers the object in the Tickable-list
    */
    Tickable::Tickable()
    {
        RegisterRootObject(Tickable);
    }
}
