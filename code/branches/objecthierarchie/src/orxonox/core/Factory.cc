#include "Factory.h"
#include "Identifier.h"
#include "BaseObject.h"

namespace orxonox
{
    Factory* Factory::pointer_s = NULL;

    Identifier* Factory::getIdentifier(const std::string& name)
    {
        if (!pointer_s)
            pointer_s = new Factory;

        return pointer_s->identifierMap_[name];
    }

    void Factory::add(const std::string& name, Identifier* identifier)
    {
        if (!pointer_s)
            pointer_s = new Factory;

        pointer_s->identifierMap_[name] = identifier;
    }
}
