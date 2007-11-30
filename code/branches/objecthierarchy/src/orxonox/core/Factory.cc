#include "Factory.h"
#include "Identifier.h"

namespace orxonox
{
    Factory* Factory::pointer_s = NULL;

    Identifier* Factory::getIdentifier(const std::string& name)
    {
        if (!pointer_s)
            pointer_s = new Factory;

        return pointer_s->identifierStringMap_[name];
    }

    Identifier* Factory::getIdentifier(const unsigned int id)
    {
        if (!pointer_s)
            pointer_s = new Factory;

        return pointer_s->identifierNetworkIDMap_[id];
    }

    void Factory::add(const std::string& name, Identifier* identifier)
    {
        if (!pointer_s)
            pointer_s = new Factory;

        pointer_s->identifierStringMap_[name] = identifier;
        pointer_s->identifierNetworkIDMap_[identifier->getNetworkID()] = identifier;
    }

    void Factory::changeNetworkID(Identifier* identifier, const unsigned int oldID, const unsigned int newID)
    {
        pointer_s->identifierNetworkIDMap_.erase(oldID);
        pointer_s->identifierNetworkIDMap_[newID] = identifier;
    }
}
